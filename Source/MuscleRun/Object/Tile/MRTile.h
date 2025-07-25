// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MRTile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileManagerActionStarted, AActor*, ThisActor);

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 에디터 노출을 시키지 않습니다.
	FOnTileManagerActionStarted OnTileGeneratedOvelaped;

public:
	UFUNCTION()
	void GetTileTransform();

	UFUNCTION()
	void SetTileTransform();

protected:
	UFUNCTION()
	void OnOverlapBeginned();

private:
	UPROPERTY(EditAnywhere, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	TArray<FMRObjectAnchorInfo> ObstacleArray;

	UPROPERTY(EditDefaultsOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	TArray<FMRObjectAnchorInfo> PropArray;

	class USceneComponent* TransformComp;

	class UBoxComponent* TriggerVolume;

};
