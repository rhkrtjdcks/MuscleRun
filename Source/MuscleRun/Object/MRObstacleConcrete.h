// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MRObsrtuctBase.h"
#include "MRObstacleConcrete.generated.h"

/**
 * 구체적인 장애물의 데미지와 기타 로직을 정의하는 클래스입니다.
 * 리팩토링 예정.
 * 
 */

UCLASS()
class MUSCLERUN_API AMRObstacleConcrete : public AMRObsrtuctBase
{
	GENERATED_BODY()

public:
	AMRObstacleConcrete();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	
};
