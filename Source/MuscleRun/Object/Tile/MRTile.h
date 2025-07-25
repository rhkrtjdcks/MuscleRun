// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MRTile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileManagerActionStarted, AActor*, ThisActor);

/*
* Ÿ�Ͽ� ������Ʈ�� ��ġ�� ������ ���� ����ü�Դϴ�.
* �� ������ �������� ������Ʈ�� ����� �����̳ʰ� ��ġ�ϵ��� �մϴ�.
*/
USTRUCT(BlueprintType)
struct FMRObjectAnchorInfo
{
	GENERATED_BODY()

	// (�߰�) ������ ����ȭ�� ���� �߰����� ������Ƽ�Դϴ�.
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
	// ������ ������ ��Ű�� �ʽ��ϴ�.
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
