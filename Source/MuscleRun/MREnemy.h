#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MREnemy.generated.h"

class UPawnSensingComponent;

UCLASS()
class MUSCLERUN_API AMREnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AMREnemy();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
};
