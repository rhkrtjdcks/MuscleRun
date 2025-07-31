// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MRSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MUSCLERUN_API UMRSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	int32 HighScore = 0;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	float MasterVolume;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	int32 GraphicsQuality;
	
	UMRSaveGame()
	{
		HighScore = 0;
		MasterVolume = 1.0f;
		GraphicsQuality = 2; // 0: 낮음, 1: 중간, 2: 높음             
	}
};
