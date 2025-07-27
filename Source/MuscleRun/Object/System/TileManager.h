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
	// ���� �ε��� ������ ���� ������
	UPROPERTY(VisibleAnywhere, Category = "Level Streaming")
	FName TriggerID;
};
