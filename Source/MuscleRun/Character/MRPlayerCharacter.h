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
	North, // +X 축으로 전진
	East,  // +Y 축으로 전진
	South, // -X 축으로 전진
	West   // -Y 축으로 전진
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

	// 코요테 타임을 위해 Jump 함수를 오버라이드합니다.
	virtual void Jump() override;
	
	// 점프 버퍼링 등의 조작감 개선 로직을 위해 Landed 함수를 오버라이드합니다.
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


	// [수정] 시작/끝 위치를 완전한 FVector가 아닌, 측면 오프셋 값(float)만 저장
	float LaneSwitchStartLateralOffset = 0.0f;
	float LaneSwitchEndLateralOffset = 0.0f;

public:
	// 오버래핑 이벤트 발생시 Obstacle에 의해 출력
	void GetDamaged(float DamageAmount);
	// 오버래핑 이벤트 발생시 Item에 의해 출력
	void ItemActivated(EItemEffectTypes ItemTypes);
};
