// Fill out your copyright notice in the Description page of Project Settings.


#include "Sys/WidgetSubSystem/MRUIManager.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"

// [����] ���尡 ������ �� ȣ��Ǵ� �� �Լ����� ��� �ʱ�ȭ �۾��� �����Ѵ�.
void UMRUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 1. �ϵ��ڵ��� ��ηκ��� ���� Ŭ������ ���� '�ε�'�Ѵ�.
	//    Initialize�� ���� �ε� �� ����ǹǷ�, �� ���������� �����ϴ�.
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
	// 1. ���� �ν��Ͻ��� �̹� ��ȿ���� Ȯ���Ѵ�.
	if (IsValid(DebugWidgetInstance))
	{
		return DebugWidgetInstance;
	}

	// 2. �ν��Ͻ��� ���ٸ�, ���� �� �������� ���� �õ�.
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && DebugWidgetClass)
	{
		DebugWidgetInstance = CreateWidget<UUserWidget>(PC, DebugWidgetClass);
		UE_LOG(LogTemp, Log, TEXT("UMRUIManager: DebugWidgetInstance Created."));
		return DebugWidgetInstance;
	}

	// [����] �α� �޽����� �� ��Ȯ�ϰ� ����
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
		// �� �α״� GetOrCreateDebugWidget ���ο��� �� �ڼ��� �����Ƿ�, �ߺ��� �� �ִ�.
		UE_LOG(LogTemp, Warning, TEXT("UMRUIManager: Toggle failed because widget is not valid."));
	}
}