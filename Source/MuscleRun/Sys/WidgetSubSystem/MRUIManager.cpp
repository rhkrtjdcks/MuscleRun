// Fill out your copyright notice in the Description page of Project Settings.


#include "Sys/WidgetSubSystem/MRUIManager.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"

// [수정] 월드가 생성된 후 호출되는 이 함수에서 모든 초기화 작업을 수행한다.
void UMRUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 1. 하드코딩된 경로로부터 위젯 클래스를 직접 '로드'한다.
	//    Initialize는 월드 로드 후 실행되므로, 이 시점에서는 안전하다.
	const FString path = TEXT("/Game/Blueprint/Widget/BP_DebugInfo.BP_DebugInfo_C");
	DebugWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(path)).LoadSynchronous();

	if (!DebugWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("UMRUIManager: Failed to load DebugWidgetClass from path: %s"), *path);
	}

	UE_LOG(LogTemp, Log, TEXT("UIManager Subsystem Initialized."));
}

UUserWidget* UMRUIManager::GetOrCreateDebugWidget()
{
	// 1. 위젯 인스턴스가 이미 유효한지 확인한다.
	if (IsValid(DebugWidgetInstance))
	{
		return DebugWidgetInstance;
	}

	// 2. 인스턴스가 없다면, 지금 이 시점에서 생성 시도.
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && DebugWidgetClass)
	{
		DebugWidgetInstance = CreateWidget<UUserWidget>(PC, DebugWidgetClass);
		UE_LOG(LogTemp, Log, TEXT("UMRUIManager: DebugWidgetInstance Created."));
		return DebugWidgetInstance;
	}

	// [수정] 로그 메시지를 더 명확하게 수정
	if (!PC) UE_LOG(LogTemp, Warning, TEXT("UMRUIManager: PlayerController is not valid."));
	if (!DebugWidgetClass) UE_LOG(LogTemp, Warning, TEXT("UMRUIManager: DebugWidgetClass is not set."));

	return nullptr;
}


void UMRUIManager::ToggleDebugWidget()
{
	UUserWidget* Widget = GetOrCreateDebugWidget();

	if (Widget)
	{
		if (Widget->IsInViewport())
		{
			Widget->RemoveFromParent();
		}
		else
		{
			Widget->AddToViewport();
		}
	}
	else
	{
		// 이 로그는 GetOrCreateDebugWidget 내부에서 더 자세히 찍히므로, 중복될 수 있다.
		UE_LOG(LogTemp, Warning, TEXT("UMRUIManager: Toggle failed because widget is not valid."));
	}
}