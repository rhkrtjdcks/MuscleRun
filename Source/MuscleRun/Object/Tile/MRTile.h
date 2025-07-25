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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile")
	TObjectPtr<class UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile")
	FTransform ObjectTransform;

	FMRObjectAnchorInfo()
	{
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
    void UpdateObstaclesFromComponents();

    UFUNCTION(CallInEditor, Category = "MRTile")
    void UpdatePropsFromComponents();

private:
	UPROPERTY(VisibleAnywhere, Category = "MRTile")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "MRTile")
	TArray<FMRObjectAnchorInfo> ObstacleArray;

	UPROPERTY(EditAnywhere, Category = "MRTile")
	TArray<FMRObjectAnchorInfo> PropArray;

	UPROPERTY()
	class USceneComponent* DefaultScene;

	UPROPERTY()
	class UBoxComponent* TriggerVolume;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> SpawnedObstacleArray;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> SpawnedPropArray;




private:
	void GenerateComponentsFromInfo(const TArray<FMRObjectAnchorInfo>& ObjectArray, TArray<TObjectPtr<UStaticMeshComponent>>& SpawnedObject, const FString& NamePrefix);

};
