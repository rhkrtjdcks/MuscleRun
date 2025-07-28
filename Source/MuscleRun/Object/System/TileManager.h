// -----------------------------------------------------------------------------
// ATileManager.h
// -----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileManager.generated.h"

class AMRTile;
class USpawnLocationComponent; // [����] �̸� ���� �ݿ�

/**
 * @brief Ÿ�ϰ� �� ���� ������ ��� ���͵��� �ϳ��� �׷����� ���� �����ϱ� ���� ����ü�Դϴ�.
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

private:
	void SpawnTile();
	void DestroyOldestTileGroup();
	void SpawnObjectsOnTile(AMRTile* TargetTile, TArray<TObjectPtr<AActor>>& OutSpawnedActors);

	// --- ���� ������ ---

	UPROPERTY(EditAnywhere, Category = "Tile Management|Config")
	TSubclassOf<AMRTile> TileClass;

	UPROPERTY(EditAnywhere, Category = "Tile Management|Config", meta = (ClampMin = "1"))
	int32 NumInitialTiles = 10;

	UPROPERTY(EditAnywhere, Category = "Tile Management|Config", meta = (ClampMin = "1"))
	int32 MinActiveTiles = 8;

	// --- Ÿ�� ���� ---
    // �����̳ʰ� �������Ʈ �����Ϳ��� ������ Ÿ�� Ŭ������
    UPROPERTY(EditDefaultsOnly, Category = "Tile Management|Tile Types")
    TSubclassOf<AMRTile> StraightTileClass;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Management|Tile Types")
    TSubclassOf<AMRTile> TurnLeftTileClass;

    UPROPERTY(EditDefaultsOnly, Category = "Tile Management|Tile Types")
    TSubclassOf<AMRTile> TurnRightTileClass;

	// --- ���� ������ ---

	UPROPERTY()
	TObjectPtr<ACharacter> PlayerCharacter;

	// [����] TQueue ��� TArray�� ����Ͽ� Ȱ��ȭ�� Ÿ�� �׷���� �����մϴ�.
	UPROPERTY()
	TArray<FTileGroup> ActiveTileGroups;

	UPROPERTY()
	FTransform NextSpawnPointTransform;

	UPROPERTY()
	TObjectPtr<AMRTile> CurrentTrackingTile;

	// [����] ���� ���� ���� Ÿ���� �ε����� ����. TArray�� �Բ� ����ϸ� �ſ� ȿ�����Դϴ�.
	int32 CurrentTrackingTileIndex = 0;
};