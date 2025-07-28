#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MRGameMode.generated.h"

UCLASS()
class MUSCLERUN_API AMRGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMRGameMode();

	// --- Public API (Game World���� ȣ��) ---
	// �ٸ� ����(ĳ����, ������ ��)�� ���� ��Ģ�� ������ �ֱ� ���� ȣ���ϴ� �Լ����Դϴ�.

	/**
	 * @brief ������ �߰��մϴ�. �����̳� �������� �� �Լ��� ȣ���մϴ�.
	 * @param ScoreToAdd �߰��� ����
	 */
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void AddScore(int32 ScoreToAdd);

	/**
	 * @brief �÷��̾ ������� �� ȣ��� �Լ��Դϴ�. ĳ������ OnDeath ��������Ʈ�� ���ε��˴ϴ�.
	 */
	void PlayerDied();

protected:
	virtual void BeginPlay() override;

private:
	/**
	 * @brief ������ ���� ���·� ��ȯ�մϴ�. (��: ī��Ʈ�ٿ� �� InProgress��)
	 */
	void StartGame();

	/**
	 * @brief ������ ���� ���·� ��ȯ�մϴ�.
	 */
	void EndGame();

	/**
	 * @brief �ð��� ���� ������ ���̵�(�ӵ�)�� ���������� �ø��ϴ�.
	 */
	void IncreaseDifficulty();

	// --- ���� ������ ---

	// ���̵��� ����ϱ������ �ð� ���� (��)
	UPROPERTY(EditDefaultsOnly, Category = "GameMode|Difficulty")
	float TimeBetweenDifficultyIncrease = 10.0f;

	// �� ���� ������ �ӵ� ����
	UPROPERTY(EditDefaultsOnly, Category = "GameMode|Difficulty")
	float SpeedIncreaseAmount = 0.1f;

	// --- ���� ������ ---

	// ���̵� ��� Ÿ�̸Ӹ� ���� �ڵ�
	FTimerHandle IncreaseDifficultyTimerHandle;
};
