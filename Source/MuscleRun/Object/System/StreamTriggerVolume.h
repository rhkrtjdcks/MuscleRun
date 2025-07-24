// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "StreamTriggerVolume.generated.h"

/**
 * 
 */
UCLASS()
class MUSCLERUN_API AStreamTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()
	
protected:
	// ���� �ε��� ������ ���� ������
	UPROPERTY(VisibleAnywhere, Category = "Level Streaming")
	FName TriggerID;
};
