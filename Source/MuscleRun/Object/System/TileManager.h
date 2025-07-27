// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileManager.generated.h"

/**
 * 
 */
UCLASS()
class MUSCLERUN_API ATileManager : public AActor
{
	GENERATED_BODY()
	
protected:
	// 현재 로딩된 레벨에 대한 포인터
	UPROPERTY(VisibleAnywhere, Category = "Level Streaming")
	FName TriggerID;
};
