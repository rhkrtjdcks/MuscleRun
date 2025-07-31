// -----------------------------------------------------------------------------
// ATileManager.h
// -----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/MRDataType.h"
#include "TileManager.generated.h"

class AMRTile;
class USpawnLocationComponent; // [수정] 이름 변경 반영

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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetTileGroupNum() { return ActiveTileGroups.Num(); }

private:
	void SpawnTile();
	void DestroyOldestTileGroup();
	void SpawnObjectsOnTile(AMRTile* TargetTile, TArray<TObjectPtr<AActor>>& OutSpawnedActors);

	// --- 설정 변수들 ---

	/*UPROPERTY(EditAnywhere, Category = "Tile Management|Config")
	TSubclassOf<AMRTile> TileClass;*/

	UPROPERTY(EditAnywhere, Category = "Tile Management|Config", meta = (ClampMin = "1"))
	int32 NumInitialTiles = 10;

	UPROPERTY(EditAnywhere, Category = "Tile Management|Config", meta = (ClampMin = "1"))
	int32 MinActiveTiles = 8;

	// --- 타일 관련 ---
    // 디자이너가 블루프린트 에디터에서 지정할 타일 클래스들
	UPROPERTY(EditAnywhere, Category = "Tile Management|Tile Types")
    TMap<ETileType, TSubclassOf<AMRTile>> TileClassMap;


	// --- 상태 변수들 ---

	UPROPERTY()
	TObjectPtr<ACharacter> PlayerCharacter;

	// [수정] TQueue 대신 TArray를 사용하여 활성화된 타일 그룹들을 관리합니다.
	TArray<FTileGroup> ActiveTileGroups;

	UPROPERTY()
	FTransform NextSpawnPointTransform;

	UPROPERTY()
	TObjectPtr<AMRTile> CurrentTrackingTile;

	// [수정] 현재 추적 중인 타일의 인덱스를 관리. TArray와 함께 사용하면 매우 효율적입니다.
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 CurrentTrackingTileIndex = 0;

	UPROPERTY()
	ETrackDirection LastTileExitDirection = ETrackDirection::North;
};