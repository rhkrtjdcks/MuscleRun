// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MRObsrtuctBase.h"

#include "GameFramework/Actor.h"
#include "MREnemyFloater.generated.h"

/*
* 움직이는 적(장애물)에 대한 클래스입니다.
* 새로운 적을 설계할 경우 Blueprint로 상속받아 각각의 메쉬를 새로 할당하는 것을 추천합니다.
*/
UCLASS(Blueprintable)
class MUSCLERUN_API AMREnemyFloater : public AMRObsrtuctBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMREnemyFloater();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetEnemySpeed() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetEnemySpeed(const float NewSpeed);

private:
	// 속도입니다. Setter, Getter API 함수를 제공합니다. (상단)
	float EnemySpeed = 10.0f;
};
