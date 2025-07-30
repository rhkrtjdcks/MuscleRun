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
	EffectComponent = CreateDefaultSubobject<UMRItemEffectManagerComponent>(TEXT("EffectComp"));
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
}

// Called to bind functionality to input
void AMRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_Left, ETriggerEvent::Triggered, this, &AMRPlayerCharacter::MoveLeft);
		Input->BindAction(IA_Right, ETriggerEvent::Triggered, this, &AMRPlayerCharacter::MoveRight);
		Input->BindAction(IA_MTJump, ETriggerEvent::Started, this, &AMRPlayerCharacter::Jump);
		Input->BindAction(IA_MTJump, ETriggerEvent::Completed, this, &AMRPlayerCharacter::StopJumping);
	}
}

void AMRPlayerCharacter::Jump()
{
	Super::Jump();
}

void AMRPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}


void AMRPlayerCharacter::OnInputJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Super::Jump();
	}
	else
	{
		StopJumping();
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