// -----------------------------------------------------------------------------
// AMRTile.h - 최종 수정본
// -----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MRTile.generated.h"

/**
 * @brief 장애물 스폰 정보를 담는 구조체입니다.
 * 런타임에 실제 액터를 스폰하기 위한 정보와 에디터에서 미리보기를 위한 메시 정보를 가집니다.
 */
USTRUCT(BlueprintType)
struct FMRObstacleSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Tile")
	FGuid ObjectID;

	// 런타임에 스폰할 실제 장애물 액터 클래스 (예: BP_SpikeObstacle)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile")
	TSubclassOf<AActor> ActorClassToSpawn;

	// 에디터에서만 보일 미리보기용 스태틱 메시
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
 * @brief 장식용 소품(Prop) 배치 정보를 담는 구조체입니다.
 * 게임플레이 로직 없이 외형만 담당하는 스태틱 메시 컴포넌트를 생성하는 데 사용됩니다.
 */
USTRUCT(BlueprintType)
struct FMRPropInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Tile")
	FGuid ObjectID;

	// 타일에 직접 배치될 스태틱 메시
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
 * @brief 무한 러너 게임의 기본 타일 액터입니다.
 * 에디터에서는 장애물과 소품의 배치를 시각적으로 지원하고,
 * 런타임에서는 ATileManager에게 장애물 스폰 정보를 제공하는 역할을 합니다.
 */
UCLASS()
class MUSCLERUN_API AMRTile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMRTile();

	/**
	 * @brief ATileManager가 이 타일 위에 스폰해야 할 장애물 정보를 가져갑니다.
	 * @return 장애물 스폰 정보 배열의 상수 참조
	 */
	const TArray<FMRObstacleSpawnInfo>& GetObstacleSpawnData() const { return ObstacleArray; }

	/**
	 * @brief 다음 타일이 생성될 위치의 Transform을 반환합니다.
	 * @return EndArrowComponent의 월드 트랜스폼
	 */
	FTransform GetEndArrowTransform() const;

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	// 에디터에서 기즈모로 변경한 장애물 위치를 데이터에 저장하는 버튼 함수
	UFUNCTION(CallInEditor, Category = "MRTile|Actions")
	void SaveGizmoChangeForObstacle();

	// 에디터에서 기즈모로 변경한 소품 위치를 데이터에 저장하는 버튼 함수
	UFUNCTION(CallInEditor, Category = "MRTile|Actions")
	void SaveGizmoChangeForProp();

	// 타일의 기본 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "MRTile|Components")
	TObjectPtr<class UStaticMeshComponent> MeshComp;


	// 타일의 시작점을 나타내는 Arrow 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "MRTile|Components")
	TObjectPtr<class UArrowComponent> StartArrowComponent;

	// 타일의 끝점(다음 타일의 시작점)을 나타내는 Arrow 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "MRTile|Components")
	TObjectPtr<class UArrowComponent> EndArrowComponent;

	// 에디터에서 장애물 미리보기를 위해 사용하는 컴포넌트 맵
	UPROPERTY()
	TMap<FGuid, TObjectPtr<UStaticMeshComponent>> IDToComponentObstacleMap;
	
	// 타일에 직접 붙는 장식용 소품 컴포넌트 맵
	UPROPERTY()
	TMap<FGuid, TObjectPtr<UStaticMeshComponent>> IDToComponentPropMap;
	
	// ObstacleArray의 데이터를 기반으로 에디터에서 미리보기 컴포넌트를 업데이트합니다.
	void UpdateObstaclePreviews();
	
	// PropArray의 데이터를 기반으로 타일에 붙는 실제 장식 컴포넌트를 업데이트합니다.
	void UpdatePropComponents();

protected:
	// 장애물 스폰 정보 배열. 런타임에 실제 액터로 스폰됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MRTile|Spawn Data", meta = (TitleProperty = "ActorClassToSpawn"))
	TArray<FMRObstacleSpawnInfo> ObstacleArray;

	// 장식용 소품 정보 배열. 스태틱 메시 컴포넌트로 타일에 직접 붙습니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MRTile|Spawn Data", meta = (TitleProperty = "StaticMesh"))
	TArray<FMRPropInfo> PropArray;
};