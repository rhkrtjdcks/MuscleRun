#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Delegates/DelegateCombinations.h"
#include "MRGameState.generated.h"


/**
 * @brief 게임의 전체적인 상태를 정의하는 열거형입니다.
 */
UENUM(BlueprintType)
enum class EMRGameState : uint8
{
	WaitingToStart, // 시작 대기
	InProgress,     // 게임 진행 중
	GameOver        // 게임 오버
};

// 델리게이트 선언

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameScoreChanged, int32, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EMRGameState, state);


UCLASS()
class MUSCLERUN_API AMRGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMRGameState();

	// --- Public API (Getters) ---
	// 다른 액터(특히 UI)들이 현재 게임 상태 정보를 읽어갈 수 있는 함수들입니다.

	UFUNCTION(BlueprintPure, Category = "GameState")
	int32 GetScore() const { return CurrentScore; }

	UFUNCTION(BlueprintPure, Category = "GameState")
	float GetGameSpeedMultiplier() const { return GameSpeedMultiplier; }

	UFUNCTION(BlueprintPure, Category = "GameState")
	EMRGameState GetCurrentState() const { return CurrentState; }

	// --- 상태 변경 함수들 (GameMode만 호출해야 함) ---
	// 이 함수들은 오직 AMRGameMode만이 호출하여 게임의 상태를 변경해야 합니다.

	void AddScore(int32 Amount);
	void SetGameSpeedMultiplier(float NewMultiplier);
	void SetCurrentState(EMRGameState NewState);

	// --- 델리게이트 변수들 (상태 및 점수 방송용) ---
	
	FOnGameScoreChanged OnScoreChanged;
	FOnGameStateChanged OnStateChanged;

private:
	// --- 상태 변수들 ---
	// 이 변수들의 실제 '값'이 모든 플레이어에게 공유됩니다.

	// 현재 점수
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	int32 CurrentScore;

	// 현재 게임 속도 배율
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	float GameSpeedMultiplier;

	// 현재 게임의 상태 (시작 대기, 진행 중, 게임 오버 등)
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	EMRGameState CurrentState;
};