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
	// 현재 로딩된 레벨에 대한 포인터
	UPROPERTY(VisibleAnywhere, Category = "Level Streaming")
	FName TriggerID;
};
