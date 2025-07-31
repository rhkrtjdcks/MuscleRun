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
	
	// 서브시스템이 초기화될 때 호출됩니다.
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 디버그 위젯을 켜거나 끕니다. PlayerController에서 호출됩니다.
	void ToggleDebugWidget();
	
private:
	// 위젯 인스턴스를 가져오거나, 없으면 생성하는 헬퍼 함수
	UUserWidget* GetOrCreateDebugWidget();

	TSubclassOf<UUserWidget> DebugWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> DebugWidgetInstance;
};
