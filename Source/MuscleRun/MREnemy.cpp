#include "MREnemy.h"
#include "MREnemyAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Character/MRPlayerCharacter.h"

AMREnemy::AMREnemy()
{
	// AI 컨트롤러(두뇌)를 우리가 만든 MREnemyAIController로 지정
	AIControllerClass = AMREnemyAIController::StaticClass();

	// 플레이어 감지용 컴포넌트(레이더) 생성
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
}

void AMREnemy::BeginPlay()
{
	Super::BeginPlay();

	// 레이더가 무언가를 감지하면 OnPawnSeen 함수를 실행하도록 연결
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AMREnemy::OnPawnSeen);
	}
}

void AMREnemy::OnPawnSeen(APawn* SeenPawn)
{
	// 감지한 대상이 없거나, 플레이어가 아니면 무시
	if (SeenPawn == nullptr || !SeenPawn->IsA(AMRPlayerCharacter::StaticClass()))
	{
		return;
	}

	// 이 캐릭터의 AI 컨트롤러(두뇌)를 가져옴
	AMREnemyAIController* EnemyController = Cast<AMREnemyAIController>(GetController());
	if (EnemyController)
	{
		// AI 컨트롤러에게 "저 플레이어를 쫓아가!" 라고 명령
		EnemyController->MoveToPlayer(SeenPawn);
	}
}
