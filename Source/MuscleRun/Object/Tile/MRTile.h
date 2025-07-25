// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MRTile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileManagerActionStarted, AActor*, ThisActor);

/*
* 타일에 오브젝트를 배치할 정보를 가진 구조체입니다.
* 이 정보를 바탕으로 컴포넌트를 만들고 디자이너가 배치하도록 합니다.
*/
USTRUCT(BlueprintType)
struct FMRObjectAnchorInfo
{
	GENERATED_BODY()

	// (추가) 완전한 동기화를 위한 추가적인 프로퍼티입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	FGuid ObjectID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile")
	TObjectPtr<class UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile")
	FTransform ObjectTransform;

	FMRObjectAnchorInfo(){
		ObjectID = FGuid::NewGuid();
		StaticMesh = nullptr;
		ObjectTransform = FTransform::Identity;
	}
};

UCLASS()
class MUSCLERUN_API AMRTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMRTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 에디터 노출을 시키지 않습니다.
	FOnTileManagerActionStarted OnTileGeneratedOvelaped;

public:
	UFUNCTION()
	FORCEINLINE FTransform GetTileTransform() const;

	UFUNCTION()
	FORCEINLINE void SetTileTransform(const FTransform& NewTransform);

protected:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnConstruction(const FTransform& Transform) override;

    UFUNCTION(CallInEditor, Category = "MRTile")
    void SaveGizmoChangeForObstacle();

    UFUNCTION(CallInEditor, Category = "MRTile")
    void SaveGizmoChangeForProp();

private:
	UPROPERTY(VisibleAnywhere, Category = "MRTile")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "MRTile")
	TArray<FMRObjectAnchorInfo> ObstacleArray;

	UPROPERTY(EditAnywhere, Category = "MRTile")
	TArray<FMRObjectAnchorInfo> PropArray;

	UPROPERTY(EditAnywhere, Category = "MRTile")
	class UArrowComponent* StartArrowComponent;

	UPROPERTY(EditAnywhere, Category = "MRTile")
	class UArrowComponent* EndArrowComponent;

	UPROPERTY()
	class USceneComponent* DefaultScene;

	UPROPERTY(VisibleAnywhere, Category = "MRTile")
	class UBoxComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere, Category = "MRTile")
	TMap<FGuid, TObjectPtr<UStaticMeshComponent>> IDToComponentObstacleMap;

	UPROPERTY(VisibleAnywhere, Category = "MRTile")
	TMap<FGuid, TObjectPtr<UStaticMeshComponent>> IDToComponentPropMap;

	// UPROPERTY()
	// TArray<TObjectPtr<UStaticMeshComponent>> SpawnedObstacleArray;

	// UPROPERTY()
	// TArray<TObjectPtr<UStaticMeshComponent>> SpawnedPropArray;




private:
	void UpdateComponentsFromInfo(const TArray<FMRObjectAnchorInfo>& ObjectArray, TMap<FGuid, TObjectPtr<UStaticMeshComponent>>& IDToComponentMap, const FString& NamePrefix);

};
