#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Delegates/DelegateCombinations.h"
#include "Object/System/TileManager.h"
#include "Data/MRDataType.h"
#include "MRGameState.generated.h"


// ��������Ʈ ����

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameScoreChanged, int32, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EMRGameState, state);


UCLASS()
class MUSCLERUN_API AMRGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMRGameState();

	void RegisterTileManager(ATileManager* Manager);

	// --- Public API (Getters) ---
	// �ٸ� ����(Ư�� UI)���� ���� ���� ���� ������ �о �� �ִ� �Լ����Դϴ�.

	UFUNCTION(BlueprintPure, Category = "GameState")
	int32 GetCurrentScore() const { return CurrentScore; }

	UFUNCTION(BlueprintPure, Category = "GameState")
	int32 GetHighScore() const { return HighScore; }

	UFUNCTION(BlueprintPure, Category = "GameState")
	float GetGameSpeedMultiplier() const { return GameSpeedMultiplier; }

	UFUNCTION(BlueprintPure, Category = "GameState")
	EMRGameState GetCurrentState() const { return CurrentState; }

	// --- ���� ���� �Լ��� (GameMode�� ȣ���ؾ� ��) ---
	// �� �Լ����� ���� AMRGameMode���� ȣ���Ͽ� ������ ���¸� �����ؾ� �մϴ�.

	void AddScore(int32 Amount);
	void SetGameSpeedMultiplier(float NewMultiplier);
	void SetCurrentState(EMRGameState NewState);

	void SaveHighScore();
	void LoadHighScore();

	// --- ��������Ʈ ������ (���� �� ���� ��ۿ�) ---
	
	FOnGameScoreChanged OnScoreChanged;
	FOnGameStateChanged OnStateChanged;

protected:
	/**
	 * @brief ���� ���忡 �����ϴ� TileManager�� ������ ����.
	 * �ٸ� ��� ���ʹ� �� ������ ���� TileManager�� �����մϴ�.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Managers")
	TObjectPtr<class ATileManager> TileManagerRef;

private:
	// --- ���� ������ ---
	// �� �������� ���� '��'�� ��� �÷��̾�� �����˴ϴ�.

	// ���� ����
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	int32 CurrentScore;

	UPROPERTY(VisibleAnywhere, Category = "GameState")
	int32 HighScore;

	// ���� ���� �ӵ� ����
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	float GameSpeedMultiplier;

	// ���� ������ ���� (���� ���, ���� ��, ���� ���� ��)
	UPROPERTY(VisibleAnywhere, Category = "GameState")
	EMRGameState CurrentState;

	
};