#include "MRGameState.h"
#include "MRSaveGame.h"
#include <Kismet/GameplayStatics.h>

AMRGameState::AMRGameState()
{
	// �������� �ʱⰪ�� �����մϴ�.
	CurrentScore = 0;
	GameSpeedMultiplier = 1.0f;
	CurrentState = EMRGameState::WaitingToStart;

	LoadHighScore(); // ���� ���� �� �ְ� ���� �ҷ�����
}

void AMRGameState::RegisterTileManager(ATileManager* Manager)
{
	TileManagerRef = Manager;
}

void AMRGameState::AddScore(int32 Amount)
{
	CurrentScore += Amount;

	if (CurrentScore > HighScore)
	{
		HighScore = CurrentScore;
		SaveHighScore(); // �ְ��� ���� �� ����
	}

	OnScoreChanged.Broadcast(CurrentScore);

	// TODO: UI�� �� ��������Ʈ�� ���ε��ؼ� ������ ������Ʈ�ϴ� ������ �ۼ��մϴ�.
}

void AMRGameState::SetGameSpeedMultiplier(float NewMultiplier)
{
	GameSpeedMultiplier = NewMultiplier;
}

void AMRGameState::SetCurrentState(EMRGameState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		OnStateChanged.Broadcast(CurrentState);
	}
}

void AMRGameState::SaveHighScore()
{
	UMRSaveGame* SaveGameInstance = Cast<UMRSaveGame>(UGameplayStatics::CreateSaveGameObject(UMRSaveGame::StaticClass()));
	if (SaveGameInstance)
	{
		SaveGameInstance->HighScore = CurrentScore;
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("DefaultSlot"), 0);
	}
}

void AMRGameState::LoadHighScore()
{
	UMRSaveGame* Loaded = Cast<UMRSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("DefaultSlot"), 0));
	if (Loaded)
	{
		Loaded->HighScore;
	}
}