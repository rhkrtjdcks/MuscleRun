// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MRDataType.generated.h"

/**
 * 
 */

// ������ Ÿ���Դϴ�. ���� ������ Ÿ�Ե��� �޾� ������ ȿ���� switch �ȿ��� �����մϴ�.
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
	North, // +X ������ ����
	East,  // +Y ������ ����
	South, // -X ������ ����
	West   // -Y ������ ����
};

UENUM(BlueprintType)
enum class ECharacterLane : uint8
{
	Left = 0,
	Center,
	Right
};

/**
 * @brief Ÿ�ϰ� �� ���� ������ ��� ���͵��� �ϳ��� �׷����� ���� �����ϱ� ���� ����ü�Դϴ�.
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
 * @brief ������ ��ü���� ���¸� �����ϴ� �������Դϴ�.
 */
UENUM(BlueprintType)
enum class EMRGameState : uint8
{
	WaitingToStart, // ���� ���
	InProgress,     // ���� ���� ��
	GameOver        // ���� ����
};