// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MRObsrtuctBase.generated.h"

/*
* 장애물이 데미지를 입히거나, 오버래핑을 일으키는 추상적 로직이 일어나는 클래스입니다.
* 메쉬나 더욱 구체화된 조절은 MRObstacleConcrete에서 하기로 합니다.
*/
UCLASS(Abstract)
class MUSCLERUN_API AMRObsrtuctBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMRObsrtuctBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Obstruct")
	void MakeDamage(float DamageAmount);

};
