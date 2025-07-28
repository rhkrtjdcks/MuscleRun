#include "MRGameState.h"

AMRGameState::AMRGameState()
{
	// 변수들의 초기값을 설정합니다.
	CurrentScore = 0;
	GameSpeedMultiplier = 1.0f;
	CurrentState = EMRGameState::WaitingToStart;
}

void AMRGameState::AddScore(int32 Amount)
{
	CurrentScore += Amount;

	OnScoreChanged.Broadcast(CurrentScore);

	// TODO: UI가 이 델리게이트에 바인딩해서 점수를 업데이트하는 로직을 작성합니다.
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
