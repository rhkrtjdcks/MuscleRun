#include "MREnemyAIController.h"

void AMREnemyAIController::MoveToPlayer(APawn* Player)
{
	if (Player)
	{
		// 네비게이션 시스템을 이용해 목표(Player)까지 이동 명령
		MoveToActor(Player, 5.f);
	}
}