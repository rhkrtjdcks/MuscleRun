#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Delegates/DelegateCombinations.h"
#include "MRGameState.generated.h"


/**
 * @brief ������ ��ü���� ���¸� �����ϴ� �������Դϴ�.
 */
UENUM(BlueprintType)
enum class EMRGameState : uint8
{
	WaitingToStart, // ���� ���
	InProgress,     // ���� ���� ��
	GameOver        // ���� ����
};

// ��������Ʈ ����

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameScoreChanged, int32, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EMRGameState, state);


UCLASS()
class MUSCLERUN_API AMRGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMRGameState();

	// --- Public API (Getters) ---
	// �ٸ� ����(Ư�� UI)���� ���� ���� ���� ������ �о �� �ִ� �Լ����Դϴ�.

	UFUNCTION(BlueprintPure, Category = "GameState")
	int32 GetScore() const { return CurrentScore; }

	UFUNCTION(BlueprintPure, Category = "GameState")
	float GetGameSpeedMultiplier() const { return GameSpeedMultiplier; }

	UFUNCTION(BlueprintPure, Category = "GameState")
	EMRGameState GetCurrentState() const { return CurrentState; }

	// --- ���� ���� �Լ��� (GameMode�� ȣ���ؾ� ��) ---
	// �� �Լ����� ���� AMRGameMode���� ȣ���Ͽ� ������ ���¸� �����ؾ� �մϴ�.

	void AddScore(int32 Amount);
	void SetGameSpeedMultiplier(float NewMultiplier);
	void SetCurrentState(EMRGameState NewState);

	// --- ��������Ʈ ������ (���� �� ���� ��ۿ�) ---
	
	FOnGameScoreChanged OnScoreChanged;
	FOnGameStateChanged OnStateChanged;

private:
	// --- ���� ������ ---
	// �� �������� ���� '��'�� ��� �÷��̾�� �����˴ϴ�.

	// ���� ����
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	int32 CurrentScore;

	// ���� ���� �ӵ� ����
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	float GameSpeedMultiplier;

	// ���� ������ ���� (���� ���, ���� ��, ���� ���� ��)
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	EMRGameState CurrentState;
};