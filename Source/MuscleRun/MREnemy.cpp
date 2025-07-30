#include "MREnemy.h"
#include "MREnemyAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Character/MRPlayerCharacter.h"

AMREnemy::AMREnemy()
{
	// AI ��Ʈ�ѷ�(�γ�)�� �츮�� ���� MREnemyAIController�� ����
	AIControllerClass = AMREnemyAIController::StaticClass();

	// �÷��̾� ������ ������Ʈ(���̴�) ����
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
}

void AMREnemy::BeginPlay()
{
	Super::BeginPlay();

	// ���̴��� ���𰡸� �����ϸ� OnPawnSeen �Լ��� �����ϵ��� ����
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AMREnemy::OnPawnSeen);
	}
}

void AMREnemy::OnPawnSeen(APawn* SeenPawn)
{
	// ������ ����� ���ų�, �÷��̾ �ƴϸ� ����
	if (SeenPawn == nullptr || !SeenPawn->IsA(AMRPlayerCharacter::StaticClass()))
	{
		return;
	}

	// �� ĳ������ AI ��Ʈ�ѷ�(�γ�)�� ������
	AMREnemyAIController* EnemyController = Cast<AMREnemyAIController>(GetController());
	if (EnemyController)
	{
		// AI ��Ʈ�ѷ����� "�� �÷��̾ �Ѿư�!" ��� ���
		EnemyController->MoveToPlayer(SeenPawn);
	}
}
