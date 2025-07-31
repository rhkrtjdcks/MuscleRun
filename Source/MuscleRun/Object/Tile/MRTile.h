// -----------------------------------------------------------------------------
// AMRTile.h - ���� ������
// -----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MRTile.generated.h"

/**
 * @brief ��ֹ� ���� ������ ��� ����ü�Դϴ�.
 * ��Ÿ�ӿ� ���� ���͸� �����ϱ� ���� ������ �����Ϳ��� �̸����⸦ ���� �޽� ������ �����ϴ�.
 */
USTRUCT(BlueprintType)
struct FMRObstacleSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Tile")
	FGuid ObjectID;

	// ��Ÿ�ӿ� ������ ���� ��ֹ� ���� Ŭ���� (��: BP_SpikeObstacle)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile")
	TSubclassOf<AActor> ActorClassToSpawn;

	// �����Ϳ����� ���� �̸������ ����ƽ �޽�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile")
	TObjectPtr<class UStaticMesh> PreviewMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile")
	FTransform ObjectTransform;

	FMRObstacleSpawnInfo()
	{
		ObjectID = FGuid::NewGuid();
		ActorClassToSpawn = nullptr;
		PreviewMesh = nullptr;
		ObjectTransform = FTransform::Identity;
	}
};

/**
 * @brief ��Ŀ� ��ǰ(Prop) ��ġ ������ ��� ����ü�Դϴ�.
 * �����÷��� ���� ���� ������ ����ϴ� ����ƽ �޽� ������Ʈ�� �����ϴ� �� ���˴ϴ�.
 */
USTRUCT(BlueprintType)
struct FMRPropInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Tile")
	FGuid ObjectID;

	// Ÿ�Ͽ� ���� ��ġ�� ����ƽ �޽�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile")
	TObjectPtr<class UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile")
	FTransform ObjectTransform;

	FMRPropInfo()
	{
		ObjectID = FGuid::NewGuid();
		StaticMesh = nullptr;
		ObjectTransform = FTransform::Identity;
	}
};

/**
 * @class AMRTile
 * @brief ���� ���� ������ �⺻ Ÿ�� �����Դϴ�.
 * �����Ϳ����� ��ֹ��� ��ǰ�� ��ġ�� �ð������� �����ϰ�,
 * ��Ÿ�ӿ����� ATileManager���� ��ֹ� ���� ������ �����ϴ� ������ �մϴ�.
 */
UCLASS()
class MUSCLERUN_API AMRTile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMRTile();

	/**
	 * @brief ATileManager�� �� Ÿ�� ���� �����ؾ� �� ��ֹ� ������ �������ϴ�.
	 * @return ��ֹ� ���� ���� �迭�� ��� ����
	 */
	const TArray<FMRObstacleSpawnInfo>& GetObstacleSpawnData() const { return ObstacleArray; }

	/**
	 * @brief ���� Ÿ���� ������ ��ġ�� Transform�� ��ȯ�մϴ�.
	 * @return EndArrowComponent�� ���� Ʈ������
	 */
	FTransform GetEndArrowTransform() const;

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// ��ֹ� ���� ���� �迭. ��Ÿ�ӿ� ���� ���ͷ� �����˴ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MRTile|Spawn Data", meta = (TitleProperty = "ActorClassToSpawn"))
	TArray<FMRObstacleSpawnInfo> ObstacleArray;

	// ��Ŀ� ��ǰ ���� �迭. ����ƽ �޽� ������Ʈ�� Ÿ�Ͽ� ���� �ٽ��ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MRTile|Spawn Data", meta = (TitleProperty = "StaticMesh"))
	TArray<FMRPropInfo> PropArray;

private:
	// �����Ϳ��� ������ ������ ��ֹ� ��ġ�� �����Ϳ� �����ϴ� ��ư �Լ�
	UFUNCTION(CallInEditor, Category = "MRTile|Actions")
	void SaveGizmoChangeForObstacle();

	// �����Ϳ��� ������ ������ ��ǰ ��ġ�� �����Ϳ� �����ϴ� ��ư �Լ�
	UFUNCTION(CallInEditor, Category = "MRTile|Actions")
	void SaveGizmoChangeForProp();

	// Ÿ���� �⺻ �޽� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = "MRTile|Components")
	TObjectPtr<class UStaticMeshComponent> MeshComp;


	// Ÿ���� �������� ��Ÿ���� Arrow ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = "MRTile|Components")
	TObjectPtr<class UArrowComponent> StartArrowComponent;

	// Ÿ���� ����(���� Ÿ���� ������)�� ��Ÿ���� Arrow ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = "MRTile|Components")
	TObjectPtr<class UArrowComponent> EndArrowComponent;

	// �����Ϳ��� ��ֹ� �̸����⸦ ���� ����ϴ� ������Ʈ ��
	UPROPERTY()
	TMap<FGuid, TObjectPtr<UStaticMeshComponent>> IDToComponentObstacleMap;
	
	// Ÿ�Ͽ� ���� �ٴ� ��Ŀ� ��ǰ ������Ʈ ��
	UPROPERTY()
	TMap<FGuid, TObjectPtr<UStaticMeshComponent>> IDToComponentPropMap;
	
	// ObstacleArray�� �����͸� ������� �����Ϳ��� �̸����� ������Ʈ�� ������Ʈ�մϴ�.
	void UpdateObstaclePreviews();
	
	// PropArray�� �����͸� ������� Ÿ�Ͽ� �ٴ� ���� ��� ������Ʈ�� ������Ʈ�մϴ�.
	void UpdatePropComponents();
};
