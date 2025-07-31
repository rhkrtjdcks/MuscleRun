// -----------------------------------------------------------------------------
// ATileManager.h
// -----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/MRDataType.h"
#include "TileManager.generated.h"

class AMRTile;
class USpawnLocationComponent; // [����] �̸� ���� �ݿ�

/**
 * @class ATileManager
 * @brief ���� ���� ������ Ÿ�� ����, �ı� �� �帧�� �Ѱ��ϴ� �߾� ������ Ŭ�����Դϴ�.
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

	// --- ���� ������ ---

	/*UPROPERTY(EditAnywhere, Category = "Tile Management|Config")
	TSubclassOf<AMRTile> TileClass;*/

	UPROPERTY(EditAnywhere, Category = "Tile Management|Config", meta = (ClampMin = "1"))
	int32 NumInitialTiles = 10;

	UPROPERTY(EditAnywhere, Category = "Tile Management|Config", meta = (ClampMin = "1"))
	int32 MinActiveTiles = 8;

	// --- Ÿ�� ���� ---
    // �����̳ʰ� �������Ʈ �����Ϳ��� ������ Ÿ�� Ŭ������
	UPROPERTY(EditAnywhere, Category = "Tile Management|Tile Types")
    TMap<ETileType, TSubclassOf<AMRTile>> TileClassMap;


	// --- ���� ������ ---

	UPROPERTY()
	TObjectPtr<ACharacter> PlayerCharacter;

	// [����] TQueue ��� TArray�� ����Ͽ� Ȱ��ȭ�� Ÿ�� �׷���� �����մϴ�.
	TArray<FTileGroup> ActiveTileGroups;

	UPROPERTY()
	FTransform NextSpawnPointTransform;

	UPROPERTY()
	TObjectPtr<AMRTile> CurrentTrackingTile;

	// [����] ���� ���� ���� Ÿ���� �ε����� ����. TArray�� �Բ� ����ϸ� �ſ� ȿ�����Դϴ�.
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 CurrentTrackingTileIndex = 0;

	UPROPERTY()
	ETrackDirection LastTileExitDirection = ETrackDirection::North;
};