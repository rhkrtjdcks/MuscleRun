// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MRPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/MRItemEffectManagerComponent.h"
#include "Object/Item/ItemBaseActor.h"
#include "Component/MRHealthComponent.h"
#include <Sys/GameState/MRGameState.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AMRPlayerCharacter::AMRPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// --- 컴포넌트 계층 구조 설정 ---

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(RootComponent);
	HealthComp = CreateDefaultSubobject<UMRHealthComponent>(TEXT("HealthComp"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// --- Bool 프로퍼티 설정 ---
	
	// 캐릭터는 컨트롤러의 회전을 따르지 않는다 (이동 방향을 바라봐야 하므로).
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// 스프링 암도 컨트롤러의 회전을 따르지 않고, 캐릭터의 등 뒤에 고정되어야 한다.
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = false; // 카메라가 벽에 부딪혀 줌인되는 것을 방지

	// 카메라는 스프링 암에 붙어있으므로, 당연히 컨트롤러 회전을 따르지 않는다.
	Camera->bUsePawnControlRotation = false;


	// --- CMC 기본 인자 설정 ---
	GetCharacterMovement()->bOrientRotationToMovement = true; // 이동 방향으로 캐릭터를 회전시킨다.
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = BASE_SPEED_MAX;
	GetCharacterMovement()->JumpZVelocity = BASE_JUMP_VELOCITY;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->GravityScale = BASE_GRAVITY_SCALE;

	// --- 추가 설정 및 초기화 ---

	// 상태 변수의 초기값을 설정합니다.
	CurrentState = EPlayerState::Idle;
}

// SetState: 캐릭터의 상태를 변경하는 유일한 함수입니다.
void AMRPlayerCharacter::SetState(EPlayerState NewState)
{
	if (CurrentState == NewState) return;

	CurrentState = NewState;
	OnStateEnter(CurrentState);
}

// Called when the game starts or when spawned
void AMRPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_MRPlayerInput, 0);
		}
	}

	CachedGameState = Cast<AMRGameState>(UGameplayStatics::GetGameState(this));

	// [수정] 게임 시작 시 Idle이 아닌 Run 상태로 바로 전환합니다.
	SetState(EPlayerState::Run);
}

// Called every frame
void AMRPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CachedGameState)
	{
		double NewMultiplier = CachedGameState->GetGameSpeedMultiplier();

		GetCharacterMovement()->MaxWalkSpeed = BASE_SPEED_MAX * NewMultiplier;
		GetCharacterMovement()->GravityScale = BASE_GRAVITY_SCALE * NewMultiplier;
		GetCharacterMovement()->JumpZVelocity = BASE_JUMP_VELOCITY * NewMultiplier * NewMultiplier;
	}

	// 1. 전진 이동: 현재 트랙 방향에 따라 올바른 방향으로 힘을 가한다.
	FVector ForwardDirection = FVector::ZeroVector;
	switch (CurrentTrackDirection)
	{
	case ETrackDirection::North: ForwardDirection = FVector::ForwardVector; break; // +X
	case ETrackDirection::East:  ForwardDirection = FVector::RightVector;   break; // +Y
	case ETrackDirection::South: ForwardDirection = -FVector::ForwardVector; break; // -X
	case ETrackDirection::West:  ForwardDirection = -FVector::RightVector;  break; // -Y
	}
	AddMovementInput(ForwardDirection, 1.0f);

	// 2. 레인 변경 로직
	if (bIsSwitchingLane)
	{
		LaneSwitchAlpha += DeltaTime / LaneSwitchDuration;
		LaneSwitchAlpha = FMath::Min(LaneSwitchAlpha, 1.0f);

		const float NewLateralOffset = FMath::Lerp(LaneSwitchStartLateralOffset, LaneSwitchEndLateralOffset, LaneSwitchAlpha);

		FVector NewLocation = GetActorLocation();

		// [단순화] 현재 트랙 방향에 따라 X 또는 Y 좌표만 직접 수정한다.
		switch (CurrentTrackDirection)
		{
		case ETrackDirection::North:
		case ETrackDirection::South:
			NewLocation.Y = NewLateralOffset;
			break;
		case ETrackDirection::East:
		case ETrackDirection::West:
			NewLocation.X = NewLateralOffset;
			break;
		}
		SetActorLocation(NewLocation);

		// 레인 변경 완료 처리
		if (LaneSwitchAlpha >= 1.0f)
		{
			bIsSwitchingLane = false;
			CurrentLane = TargetLane;

			// 최종 위치 보정
			NewLocation = GetActorLocation();
			switch (CurrentTrackDirection)
			{
			case ETrackDirection::North:
			case ETrackDirection::South:
				NewLocation.Y = LaneSwitchEndLateralOffset;
				break;
			case ETrackDirection::East:
			case ETrackDirection::West:
				NewLocation.X = LaneSwitchEndLateralOffset;
				break;
			}
			SetActorLocation(NewLocation);
		}
	}

	// [추가] 상태 머신 업데이트 로직을 매 프레임 호출합니다.
	OnStateUpdate(DeltaTime);
}

// Called to bind functionality to input
void AMRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_Left, ETriggerEvent::Triggered, this, &AMRPlayerCharacter::MoveLeft);
		Input->BindAction(IA_Right, ETriggerEvent::Triggered, this, &AMRPlayerCharacter::MoveRight);
		Input->BindAction(IA_MTJump, ETriggerEvent::Triggered, this, &AMRPlayerCharacter::OnInputJump);
	}
}

// [추가] 상태 진입 시 실행할 로직을 추가합니다.
void AMRPlayerCharacter::OnStateEnter(EPlayerState State)
{
	switch (State)
	{
	case EPlayerState::Run:
		// 달리기 상태에 진입하면 캐릭터의 최고 속도를 설정합니다.
		GetCharacterMovement()->MaxWalkSpeed = BASE_SPEED_MAX;
		break;
	case EPlayerState::Jump:
		// [수정] ACharacter의 내장 점프 함수를 여기서 직접 호출합니다.
		Super::Jump();

		// [추가] 점프와 동시에 실행하고 싶은 추가 로직(사운드, 이펙트 등)도 여기에 넣습니다.
		// 예시: UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());
		break;

		// ... 다른 상태에 대한 진입 로직
	default:
		break;
	}
}

// [추가] 상태에 따라 매 프레임 실행할 로직을 연결합니다.
void AMRPlayerCharacter::OnStateUpdate(float DeltaTime)
{
	switch (CurrentState)
	{
	case EPlayerState::Idle:		UpdateIdle(DeltaTime); break;
	case EPlayerState::Run:			UpdateRun(DeltaTime); break;
	case EPlayerState::Jump:		UpdateJump(DeltaTime); break;
	case EPlayerState::Sliding:		UpdateSliding(DeltaTime); break;
	default:
		break;
	}
}

void AMRPlayerCharacter::UpdateIdle(float DeltaTime)
{
}

void AMRPlayerCharacter::UpdateRun(float DeltaTime)
{
}

void AMRPlayerCharacter::UpdateJump(float DeltaTime)
{
}

void AMRPlayerCharacter::UpdateSliding(float DeltaTime)
{
}


void AMRPlayerCharacter::Jump()
{

}

// [수정] 땅에 착지하면 Run 상태로 전환합니다.
void AMRPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	SetState(EPlayerState::Run);
}

// [수정] 점프 입력을 받으면 상태를 Jump로 변경합니다.
void AMRPlayerCharacter::OnInputJump(const FInputActionValue& Value)
{
	// 공중에 떠 있는 상태(점프, 낙하)가 아닐 때만 점프를 허용합니다.
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		SetState(EPlayerState::Jump);
	}
}

void AMRPlayerCharacter::MoveLeft()
{
	if (bIsSwitchingLane) return;
	int32 NewLaneIndex = FMath::Max(0, static_cast<int32>(CurrentLane) - 1);
	TargetLane = static_cast<ECharacterLane>(NewLaneIndex);
	if (TargetLane != CurrentLane)
	{
		StartLaneSwitch();
	}
}

void AMRPlayerCharacter::MoveRight()
{
	if (bIsSwitchingLane) return;
	int32 NewLaneIndex = FMath::Min(2, static_cast<int32>(CurrentLane) + 1);
	TargetLane = static_cast<ECharacterLane>(NewLaneIndex);
	if (TargetLane != CurrentLane)
	{
		StartLaneSwitch();
	}
}

void AMRPlayerCharacter::StartLaneSwitch()
{
	bIsSwitchingLane = true;
	LaneSwitchAlpha = 0.0f;

	const FVector CurrentLocation = GetActorLocation();

	// [단순화] 현재 트랙 방향에 따라 어떤 축(X 또는 Y)을 측면 축으로 사용할지 결정
	switch (CurrentTrackDirection)
	{
	case ETrackDirection::North:
	case ETrackDirection::South:
		LaneSwitchStartLateralOffset = CurrentLocation.Y;
		break;
	case ETrackDirection::East:
	case ETrackDirection::West:
		LaneSwitchStartLateralOffset = CurrentLocation.X;
		break;
	}

	const int32 LogicalLaneIndex = static_cast<int32>(TargetLane) - 1; // 0,1,2 -> -1,0,1
	LaneSwitchEndLateralOffset = LogicalLaneIndex * LaneWidth;
}

void AMRPlayerCharacter::GetDamaged(float DamageAmount)
{
	// Implement damage logic here
	HealthComp->GetDamage(DamageAmount);
}

void AMRPlayerCharacter::ItemActivated(EItemEffectTypes ItemTypes)
{

	// Implement item activation logic here
	if (EffectComponent)
	{
		EffectComponent->ApplyEffect(ItemTypes);
	}
}