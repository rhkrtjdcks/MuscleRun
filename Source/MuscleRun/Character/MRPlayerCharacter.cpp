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
#include "../Sys/WidgetSubSystem/MRUIManager.h"

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
	GetWorld()->GetSubsystem<UMRUIManager>()->ToggleDebugWidget();

	if (ATileManager* TileManager = Cast<ATileManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATileManager::StaticClass())))
	{
		// "나(캐릭터)의 Tick은, 반드시 TileManager의 Tick이 끝난 후에 실행되어야 한다"
		// 라고 엔진에게 명시적으로 알려줍니다.
		AddTickPrerequisiteActor(TileManager);
	}
}

// Called every frame
void AMRPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CachedGameState)
	{
		double NewMultiplier = CachedGameState->GetGameSpeedMultiplier();

		GetCharacterMovement()->MaxWalkSpeed = BASE_SPEED_MAX * NewMultiplier;
		GetCharacterMovement()->GravityScale = BASE_GRAVITY_SCALE * NewMultiplier * NewMultiplier;
		GetCharacterMovement()->JumpZVelocity = BASE_JUMP_VELOCITY * NewMultiplier;
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
			NewLocation.Y = FixedLaneOffset + NewLateralOffset;
			break;
		case ETrackDirection::South:
			NewLocation.Y = FixedLaneOffset - NewLateralOffset;
			break;
		case ETrackDirection::East:
			NewLocation.X = FixedLaneOffset - NewLateralOffset;
			break;
		case ETrackDirection::West:
			NewLocation.X = FixedLaneOffset + NewLateralOffset;
			break;
		}
		UE_LOG(LogTemp, Log, TEXT("Movement Is Here, NewLateral : %.2f, Current Start Offset : %.2f, Current End Offset : %.2f"), NewLateralOffset, LaneSwitchStartLateralOffset, LaneSwitchEndLateralOffset);
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
				NewLocation.Y = FixedLaneOffset + LaneSwitchEndLateralOffset;
				break;
			case ETrackDirection::South:
				NewLocation.Y = FixedLaneOffset - LaneSwitchEndLateralOffset;
				break;
			case ETrackDirection::East:
				NewLocation.X = FixedLaneOffset - LaneSwitchEndLateralOffset;
				break;
			case ETrackDirection::West:
				NewLocation.X = FixedLaneOffset + LaneSwitchEndLateralOffset;
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


void AMRPlayerCharacter::ExecuteForceTurn(const FTransform& AlignmentTransform, ETrackDirection NewDirection)
{
	// 1. [유지] 논리적 레인을 중앙으로 강제 초기화한다.
	//    이건 회전 후의 혼란을 막기 위해 여전히 유용하다.
	CurrentLane = ECharacterLane::Center;
	TargetLane = ECharacterLane::Center;

	// 2. [수정] 캐릭터의 회전을 새로운 타일의 방향과 즉시 '일치'시킨다.
	//    부드러운 회전은 나중에 추가하더라도, 일단은 바로 꺾어준다.
	const FRotator NewRotation = AlignmentTransform.GetRotation().Rotator();
	SetActorRotation(NewRotation);
	// 컨트롤러의 회전도 함께 맞춰주는 것이 좋다.
	if (AController* PC = GetController())
	{
		PC->SetControlRotation(NewRotation);
	}

	// 3. [유지] 새로운 트랙 방향을 상태 변수에 저장한다.
	CurrentTrackDirection = NewDirection;
	FVector CurrentLocation = GetActorLocation();

	// 1. 현재 캐릭터의 속도를 가져온다.
	FVector CurrentVelocity = GetCharacterMovement()->Velocity;

	// 2. 새로운 트랙의 '앞'과 '오른쪽' 방향 벡터를 가져온다.
	const FVector NewForwardVector = GetActorForwardVector();
	const FVector NewRightVector = GetActorRightVector();

	// 3. 현재 속도를 '새로운 앞 방향'에 투영하여 전진 속도 성분만 남긴다.
	//    (옆으로 밀려나던 속도는 이 과정에서 제거된다)
	FVector NewVelocity = FVector::DotProduct(CurrentVelocity, NewForwardVector) * NewForwardVector;

	// 4. (선택) 중력으로 인한 수직 속도는 유지해주는 것이 좋다.
	NewVelocity.Z = CurrentVelocity.Z;

	// 5. 계산된 '깨끗한' 전진 속도를 캐릭터 무브먼트 컴포넌트에 다시 설정한다.
	GetCharacterMovement()->Velocity = NewVelocity;

	switch (NewDirection)
	{
	case ETrackDirection::North:
	case ETrackDirection::South:
		FixedLaneOffset = CurrentLocation.Y;
		break;
	case ETrackDirection::West:
	case ETrackDirection::East:
		FixedLaneOffset = CurrentLocation.X;
		break;
	default:
		break;
	}

	LaneSwitchEndLateralOffset = 0;
	LaneSwitchStartLateralOffset = 0;

	UE_LOG(LogTemp, Log, TEXT("Control Axis Rotated. New direction: %s"), *UEnum::GetValueAsString(NewDirection));
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

	LaneSwitchStartLateralOffset = LaneSwitchEndLateralOffset;

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