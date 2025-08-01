﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Component/MRItemEffectManagerComponent.h"
#include "InputActionValue.h"
#include "../Sys/GameState/MRGameState.h"
#include "Component/MRHealthComponent.h"
#include "Data/MRDataType.h"
#include "UObject/NoExportTypes.h"
#include "MRPlayerCharacter.generated.h"


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Running		UMETA(DisplayName = "Running"),
	ECS_Jumping		UMETA(DisplayName = "Jumping"),
	ECS_Sliding		UMETA(DisplayName = "Sliding")
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* JumpMontage;

	// 현재 추적하고 있는 방향입니다. 이동 로직에서 사용합니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Movement|Lanes")
	ETrackDirection CurrentTrackDirection = ETrackDirection::North;

	// 현재 체력을 반환하는 Public API 함수입니다.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHealth(){ return HealthComp->RetHealth(); }

	// 회전 타일을 만났을 때 강제 회전
	void ExecuteForceTurn(const FTransform& PlaneOrigin, const ETrackDirection TileEndDirection);

	// 오버래핑 이벤트 발생시 Obstacle에 의해 출력
	void GetDamaged(float DamageAmount);

	// 오버래핑 이벤트 발생시 Item에 의해 출력
	void ItemActivated(EItemEffectTypes ItemTypes);

protected:
	// --- 컴포넌트 관련 ---

	UPROPERTY(VisibleAnywhere, Category = "Player|Comps")
	class UBoxComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere, Category = "Player|Comps")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category ="Player|Comps")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Player|Comps")
	class UMRHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, Category = "Player|Comps")
	class UMRItemEffectManagerComponent* EffectComponent;


	// --- 입력 매핑 관련 ---

	UPROPERTY(EditAnywhere, Category = "Player|Input")
	class UInputMappingContext* IMC_MRPlayerInput;

	UPROPERTY(EditAnywhere, Category = "Player|Input")
	class UInputAction* IA_Left;

	UPROPERTY(EditAnywhere, Category = "Player|Input")
	class UInputAction* IA_Right;

	UPROPERTY(EditAnywhere, Category = "Player|Input")
	class UInputAction* IA_MTJump;

	UPROPERTY(EditAnywhere, Category = "Player|Input")
	class UInputAction* IA_Slide;

	UPROPERTY(EditAnywhere, Category = "Player|Input")
	class UInputAction* IA_ToggleWidget;

	UPROPERTY(EditAnywhere, Category = "Player|Input")
	class UInputAction* IA_Escape;

	// --- 논리적 레인 기반 이동 관련 함수 및 변수 ---

	void MoveLeft();

	void MoveRight();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Movement|Lane")
	ECharacterLane CurrentLane = ECharacterLane::Center;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Movement|Lane")
	ECharacterLane TargetLane = ECharacterLane::Center;

	// [수정] 시작/끝 위치를 완전한 FVector가 아닌, 측면 오프셋 값(float)만 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Movement|Lane")
	float LaneSwitchStartLateralOffset = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Movement|Lane")
	float LaneSwitchEndLateralOffset = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Movement|Lane")
	float FixedLaneOffset = 0.f;


	void OnInputJump(const FInputActionValue& Value);

private:
	

	// 게임 난이도 값을 받아오기 위한 MRGameState의 포인터입니다.
	UPROPERTY()
    TObjectPtr<AMRGameState> CachedGameState;


	// --- 이동 로직 관련 설정값들입니다 ---
	
	void StartLaneSwitch();

	UPROPERTY(EditAnywhere, Category = "Player|Movement|Lanes")
	float LaneWidth = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Player|Movement|Lanes")
	float LaneSwitchDuration = 0.2f;

	bool bIsSwitchingLane = false;
	float LaneSwitchAlpha = 0.0f;

	const float BASE_SPEED_MAX = 600.f;
	const float BASE_JUMP_VELOCITY = 700.f;
	const float BASE_GRAVITY_SCALE = 2.f;


	// --- 슬라이딩 기능 구현 함수 --

	void StartSlide();
	void StopSlide();

	FTimerHandle SlideTimeHandler;

	UPROPERTY()
	float DefualtCapsuleHalfSize;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Movement|Slide")
	float SlideDuration = 2.f;

	// --- 점프 버퍼링 관련 ---

	FTimerHandle JumpBufferTimerHandler;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Movement|Jump", meta = (AllowPrivateAccess = "true"))
	bool bWantsToJump = false;

	void ClearJumpBuffer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement|Jump", meta = (AllowPrivateAccess = "true"))
	float JumpBufferDuration = 0.4f;

	//캐릭터 상태변경 함수
protected:
	// 입력과 관련된 함수들
	void Slide();
	void StopSliding();

	// 캐릭터의 이동 상태가 바뀔 때 호출되는 기본 함수를 오버라이드합니다. 점프 상태를 감지하는 데 매우 유용합니다.
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	// 현재 캐릭터 상태를 저장할 변수입니다.
	// BlueprintReadOnly는 블루프린트에서 이 값을 읽을 수만 있게 하여 안정성을 높입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	ECharacterState CharacterState;

public:
	// 애니메이션 블루프린트에서 현재 상태를 직접 가져갈 수 있도록 public 함수를 만듭니다.
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
