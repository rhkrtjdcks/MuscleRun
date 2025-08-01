// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MRSaveManager.generated.h"

class UMRSaveGame;
/**
 * 
 */
UCLASS()
class MUSCLERUN_API UMRSaveManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// ����ý����� ó�� ������ �� ȣ���
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadGame();

	UFUNCTION(BlueprintPure, Category = "Save System")
	int32 GetHighScore() const;

	UFUNCTION(BlueprintCallable, Category = "Save System")
	bool UpdateHighScore(int32 NewScore); 

private:
	// ���� �޸𸮿� �ε�� SaveGame ��ü�� �ν��Ͻ�
	UPROPERTY()
	TObjectPtr<UMRSaveGame> CurrentSaveGame;
	// ���� ������ �̸��� �ε���. ����� �����ϴ� ���� ����.
	const FString SaveSlotName = TEXT("DefaultSaveSlot");
	const int32 UserIndex = 0;
};
