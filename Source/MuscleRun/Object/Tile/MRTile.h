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
