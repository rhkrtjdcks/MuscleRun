#include "MREnemyAIController.h"

void AMREnemyAIController::MoveToPlayer(APawn* Player)
{
	if (Player)
	{
		// �׺���̼� �ý����� �̿��� ��ǥ(Player)���� �̵� ���
		MoveToActor(Player, 5.f);
	}
}