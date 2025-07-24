// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interface/Obstructable.h"

#include "GameFramework/Actor.h"
#include "MREnemyFloater.generated.h"

UCLASS()
class MUSCLERUN_API AMREnemyFloater : public AActor, public IObstructable
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

};
