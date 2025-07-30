	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Component/MRItemEffectManagerComponent.h"
#include "InputActionValue.h"
#include "../Sys/GameState/MRGameState.h"
#include "MRPlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class ETrackDirection : uint8
{
	North, // +X ������ ����
	East,  // +Y ������ ����
	South, // -X ������ ����
	West   // -Y ������ ����
};

UENUM(BlueprintType)
enum class ECharacterLane : uint8
{
	Left = 0,
	Center,
	Right
};

UCLASS()
class MUSCLERUN_API AMRPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMRPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �ڿ��� Ÿ���� ���� Jump �Լ��� �������̵��մϴ�.
	virtual void Jump() override;
	
	// ���� ���۸� ���� ���۰� ���� ������ ���� Landed �Լ��� �������̵��մϴ�.
	virtual void Landed(const FHitResult& Hit) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Lanes")
	ETrackDirection CurrentTrackDirection = ETrackDirection::North;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Player")
	class UBoxComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category ="Player")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Player")
	class UInputMappingContext* IMC_MRPlayerInput;

	UPROPERTY(EditAnywhere, Category = "Player")
	class UInputAction* IA_Left;

	UPROPERTY(EditAnywhere, Category = "Player")
	class UInputAction* IA_Right;

	UPROPERTY(EditAnywhere, Category = "Player")
	class UInputAction* IA_MTJump;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	class UMRHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	class UMRItemEffectManagerComponent* EffectComponent;

	void OnInputJump(const FInputActionValue& Value);

	void MoveLeft();
	void MoveRight();

private:

	void StartLaneSwitch();

	UPROPERTY(EditAnywhere, Category = "Movement|Lanes")
	float LaneWidth = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Movement|Lanes")
	float LaneSwitchDuration = 0.2f;

	UPROPERTY()
    TObjectPtr<AMRGameState> CachedGameState;

	ECharacterLane CurrentLane = ECharacterLane::Center;
	ECharacterLane TargetLane = ECharacterLane::Center;
	bool bIsSwitchingLane = false;
	float LaneSwitchAlpha = 0.0f;

	const float BASE_SPEED_MAX = 600.f;
	const float BASE_JUMP_VELOCITY = 700.f;
	const float BASE_GRAVITY_SCALE = 2.f;


	// [����] ����/�� ��ġ�� ������ FVector�� �ƴ�, ���� ������ ��(float)�� ����
	float LaneSwitchStartLateralOffset = 0.0f;
	float LaneSwitchEndLateralOffset = 0.0f;

public:
	// �������� �̺�Ʈ �߻��� Obstacle�� ���� ���
	void GetDamaged(float DamageAmount);
	// �������� �̺�Ʈ �߻��� Item�� ���� ���
	void ItemActivated(EItemEffectTypes ItemTypes);
};
