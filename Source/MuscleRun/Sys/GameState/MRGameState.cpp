#include "MRGameState.h"

AMRGameState::AMRGameState()
{
	// �������� �ʱⰪ�� �����մϴ�.
	CurrentScore = 0;
	GameSpeedMultiplier = 1.0f;
	CurrentState = EMRGameState::WaitingToStart;
}

void AMRGameState::AddScore(int32 Amount)
{
	CurrentScore += Amount;

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
