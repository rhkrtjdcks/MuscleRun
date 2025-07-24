// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MRLevelStreamSubSystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelLoadCompleted, FName, LevelName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUnLoadCompleted, FName, LevelName);

/**
 * 레벨 스트리밍입니다.
 * 맵을 프리로딩하는 로직을 담고 있습니다.
 */
UCLASS()
class MUSCLERUN_API UMRLevelStreamSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// 레벨을 로드하는 API 함수입니다.
	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void RequestLevelLoad(FName LevelToLoad);
	
	// 레벨을 언로드하는 API 함수입니다.
	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void RequestLevelUnload(FName LevelToUnload);

private:
    // 현재 로드 중이거나, 로드가 완료된 레벨들의 상태를 추적합니다.
    UPROPERTY()
    TSet<TSoftObjectPtr<UWorld>> LevelCurrentlyLoading;

	// 로드가 완료된 레벨들의 상태를 추적합니다.
	UPROPERTY()
	TSet<TSoftObjectPtr<UWorld>> LevelCurrentlyLoaded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> LoadedLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> LoadingLevel;
};
