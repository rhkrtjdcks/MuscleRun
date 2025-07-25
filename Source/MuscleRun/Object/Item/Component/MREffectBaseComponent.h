// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MREffectBaseComponent.generated.h"

/*
* 효과를 적용하는 컴포넌트입니다.
* 이 컴포넌트는 Player의 속성을 변경시킵니다.
* ItemBaseActor 함수의 인자로 받는 OtherActor의 속성을 변경시키는 방식입니다.
*/
UCLASS(Blueprintable)
class MUSCLERUN_API UMREffectBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMREffectBaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	UPROPERTY(EditDefaultsOnly ,BlueprintReadOnly, Category = "Test")
	float Test;

	UFUNCTION(BlueprintCallable)
	void testFunction();
		
};
