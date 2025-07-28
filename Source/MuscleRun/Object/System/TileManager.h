// -----------------------------------------------------------------------------
// ATileManager.h
// -----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileManager.generated.h"

class AMRTile;
class USpawnLocationComponent; // [수정] 이름 변경 반영

/**
 * @brief 타일과 그 위에 스폰된 모든 액터들을 하나의 그룹으로 묶어 관리하기 위한 구조체입니다.
 */
USTRUCT()
struct FTileGroup
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AMRTile> Tile;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> ContainedActors;

	FTileGroup() : Tile(nullptr) {}
};

/**
 * @class ATileManager
 * @brief 무한 러너 게임의 타일 생성, 파괴 및 흐름을 총괄하는 중앙 관리자 클래스입니다.
 */
UCLASS()
class ATileManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATileManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void SpawnTile();
	void DestroyOldestTileGroup();
	void SpawnObjectsOnTile(AMRTile* TargetTile, TArray<TObjectPtr<AActor>>& OutSpawnedActors);

	// --- 설정 변수들 ---

	UPROPERTY(EditAnywhere, Category = "Tile Management|Config")
	TSubclassOf<AMRTile> TileClass;

	UPROPERTY(EditAnywhere, Category = "Tile Management|Config", meta = (ClampMin = "1"))
	int32 NumInitialTiles = 10;

	UPROPERTY(EditAnywhere, Category = "Tile Management|Config", meta = (ClampMin = "1"))
	int32 MinActiveTiles = 8;

	// --- 타일 관련 ---
    // 디자이너가 블루프린트 에디터에서 지정할 타일 클래스들
    UPROPERTY(EditDefaultsOnly, Category = "Tile Management|Tile Types")
    TSubclassOf<AMRTile> StraightTileClass;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Management|Tile Types")
    TSubclassOf<AMRTile> TurnLeftTileClass;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Management|Tile Types")
    TSubclassOf<AMRTile> TurnRightTileClass;

	// --- 상태 변수들 ---

	UPROPERTY()
	TObjectPtr<ACharacter> PlayerCharacter;

	// [수정] TQueue 대신 TArray를 사용하여 활성화된 타일 그룹들을 관리합니다.
	UPROPERTY()
	TArray<FTileGroup> ActiveTileGroups;

	UPROPERTY()
	FTransform NextSpawnPointTransform;

	UPROPERTY()
	TObjectPtr<AMRTile> CurrentTrackingTile;

	// [수정] 현재 추적 중인 타일의 인덱스를 관리. TArray와 함께 사용하면 매우 효율적입니다.
	int32 CurrentTrackingTileIndex = 0;
};