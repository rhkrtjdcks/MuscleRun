// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MRUIManager.generated.h"

/**
 * 
 */
UCLASS()
class MUSCLERUN_API UMRUIManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	// ����ý����� �ʱ�ȭ�� �� ȣ��˴ϴ�.
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// ����� ������ �Ѱų� ���ϴ�. PlayerController���� ȣ��˴ϴ�.
	void ToggleDebugWidget();
	
private:
	// ���� �ν��Ͻ��� �������ų�, ������ �����ϴ� ���� �Լ�
	UUserWidget* GetOrCreateDebugWidget();

	TSubclassOf<UUserWidget> DebugWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> DebugWidgetInstance;
};
