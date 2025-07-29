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

// Sets default values
AMRPlayerCharacter::AMRPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(RootComponent);

	SkeletonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	GetMesh()->SetupAttachment(RootComponent);

	HealthComp = CreateDefaultSubobject<UMRHealthComponent>(TEXT("HealthComp"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // 카메라는 SpringArm의 회전을 따르므로 직접 회전하지 않음


	bUseControllerRotationYaw = false;
	// GetCharacterMovement()->bUseControllerDesiredRotation = true; // 이 옵션은 bOrientRotationToMovement와 함께 사용하지 않는 것이 일반적입니다.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->GravityScale = 2.0f;
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
}

// Called every frame
void AMRPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_MRMove, ETriggerEvent::Triggered, this, &AMRPlayerCharacter::OnInputMove);
		Input->BindAction(IA_MTJump, ETriggerEvent::Started, this, &AMRPlayerCharacter::Jump);
		Input->BindAction(IA_MTJump, ETriggerEvent::Completed, this, &AMRPlayerCharacter::StopJumping);
	}
}

void AMRPlayerCharacter::OnInputMove(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (Controller && MoveVector != FVector2D::ZeroVector)
	{
		// 컨트롤러(카메라)의 회전 값을 기준으로 방향을 계산합니다.
		// 캐릭터의 Pitch나 Roll에 영향을 받지 않도록 Yaw 값만 사용합니다.
		const FRotator ControlRot = Controller->GetControlRotation();
		const FRotator YawRot(0, ControlRot.Yaw, 0);

		// 컨트롤러 기준의 Forward 방향과 Right 방향을 구합니다.
		const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

		// 해당 방향으로 이동 입력을 추가합니다.
		AddMovementInput(Forward, MoveVector.Y);
		AddMovementInput(Right, MoveVector.X);
	}
}

void AMRPlayerCharacter::OnInputJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump();
	}
	else
	{
		StopJumping();
	}
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
		UE_LOG(LogTemp, Warning, TEXT("충돌된 아이템 타입: %d"), static_cast<int32>(ItemTypes));
		EffectComponent->ApplyEffect(ItemTypes);
	}
}