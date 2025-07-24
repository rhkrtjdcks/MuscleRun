// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MRObsrtuctBase.h"

#include "GameFramework/Actor.h"
#include "MREnemyFloater.generated.h"

/*
* 움직이는 적(장애물)에 대한 클래스입니다.
* 가독성을 위해, 또 구현이 직관적이기에 (종류가 하나뿐일 것)
* 한 개의 클래스로 압축했습니다.
* Blueprint로 상속받아 각각의 메쉬를 새로 할당하는 것을 추천합니다.
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

protected:
	virtual void MakeDamage_Implementation(float DamageAmount) override;
};
