#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MREnemyAIController.generated.h"

UCLASS()
class MUSCLERUN_API AMREnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	void MoveToPlayer(APawn* Player);
};