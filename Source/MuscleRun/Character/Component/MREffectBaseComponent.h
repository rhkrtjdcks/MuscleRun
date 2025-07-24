// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MREffectBaseComponent.generated.h"


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
