// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MRDataType.generated.h"

/**
 * 
 */

// 아이템 타입입니다. 여기 정의한 타입들을 받아 아이템 효과를 switch 안에서 구현합니다.
UENUM(BlueprintType)
enum class EItemEffectTypes : uint8
{
	None,
	Score,
	Faster
};

UENUM(BlueprintType)
enum class ETrackDirection : uint8
{
	North, // +X 축으로 전진
	East,  // +Y 축으로 전진
	South, // -X 축으로 전진
	West   // -Y 축으로 전진
};

UENUM(BlueprintType)
enum class ECharacterLane : uint8
{
	Left = 0,
	Center,
	Right
};

/**
 * @brief 타일과 그 위에 스폰된 모든 액터들을 하나의 그룹으로 묶어 관리하기 위한 구조체입니다.
 */
USTRUCT()
struct FTileGroup
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<class AMRTile> Tile;

	UPROPERTY()
	TArray<TObjectPtr<class AActor>> ContainedActors;

	bool bIsTurnTrigger = false;

	ETrackDirection ExitDirection = ETrackDirection::North;

	FTileGroup() : Tile(nullptr) {}
};

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Straight,
	TurnLeft,
	TurnRight
};

/**
 * @brief 게임의 전체적인 상태를 정의하는 열거형입니다.
 */
UENUM(BlueprintType)
enum class EMRGameState : uint8
{
	WaitingToStart, // 시작 대기
	InProgress,     // 게임 진행 중
	GameOver        // 게임 오버
};