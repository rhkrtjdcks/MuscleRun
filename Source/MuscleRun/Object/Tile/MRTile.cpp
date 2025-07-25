// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Tile/MRTile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
AMRTile::AMRTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultScene = CreateDefaultSubobject<USceneComponent>("DefaultSceneComponent");
	SetRootComponent(DefaultScene);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp_TileEntire");
	MeshComp->SetupAttachment(DefaultScene);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>("TriggerVolume");
	TriggerVolume->SetupAttachment(DefaultScene);
}

// Called when the game starts or when spawned
void AMRTile::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AMRTile::OnOverlapBegin);

	// 생성 시 오브젝트에서 해야 할 효과들을 추후 여기서 지정합니다.
}

void AMRTile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnTileGeneratedOvelaped.Broadcast(this);
}

void AMRTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GenerateComponentsFromInfo(ObstacleArray, SpawnedObstacleArray, TEXT("Obstacle"));
	GenerateComponentsFromInfo(PropArray, SpawnedPropArray, TEXT("Prop"));
}

void AMRTile::UpdateObstaclesFromComponents()
{
	// Spawned 배열과 데이터 배열의 개수가 같은지 확인 (안전장치)
	if (SpawnedObstacleArray.Num() != ObstacleArray.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Obstacle component count and data count do not match. Re-run OnConstruction."));
		return;
	}

	for (int32 i = 0; i < SpawnedObstacleArray.Num(); ++i)
	{
		if (IsValid(SpawnedObstacleArray[i]))
		{
			// 컴포넌트의 현재 트랜스폼을 가져와 데이터 배열에 저장합니다.
			ObstacleArray[i].ObjectTransform = SpawnedObstacleArray[i]->GetRelativeTransform();
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Obstacle Array data has been updated from components."));
}

void AMRTile::UpdatePropsFromComponents()
{
	if (SpawnedPropArray.Num() != PropArray.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Prop component count and data count do not match. Re-run OnConstruction."));
		return;
	}
	for (int32 i = 0; i < SpawnedPropArray.Num(); ++i)
	{
		if (IsValid(SpawnedPropArray[i]))
		{
			PropArray[i].ObjectTransform = SpawnedPropArray[i]->GetRelativeTransform();
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Obstacle Array data has been updated from components."));
}

void AMRTile::GenerateComponentsFromInfo(const TArray<FMRObjectAnchorInfo>& ObjectInfoArray, TArray<TObjectPtr<UStaticMeshComponent>>& SpawnedObjectArray, const FString& NamePrefix)
{
	for (TObjectPtr<UStaticMeshComponent> Comp : SpawnedObjectArray)
	{
		if (IsValid(Comp))
		{
			Comp->DestroyComponent();
		}
	}
	SpawnedObjectArray.Empty();

	for (int32 i = 0; i < ObjectInfoArray.Num(); ++i)
	{
		const FMRObjectAnchorInfo ObjectInfo = ObjectInfoArray[i];
		if (IsValid(ObjectInfo.StaticMesh))
		{
			TObjectPtr<UStaticMeshComponent> NewMeshComp = Cast<UStaticMeshComponent>(
				AddComponentByClass(
					UStaticMeshComponent::StaticClass(),
					false,
					ObjectInfo.ObjectTransform,
					false
				)
			);

			if (ensure(NewMeshComp))
			{
				NewMeshComp->Rename(*FString::Printf(TEXT("SpawnedMesh_%s_%d"), *NamePrefix, i));
				NewMeshComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
				//NewMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				//NewMeshComp->SetRelativeTransform(ObjectInfo.ObjectTransform);
				NewMeshComp->SetStaticMesh(ObjectInfo.StaticMesh);
				//NewMeshComp->RegisterComponent();
				SpawnedObjectArray.Add(NewMeshComp);
			}
		}
	}
}

FORCEINLINE FTransform AMRTile::GetTileTransform() const
{
	return GetActorTransform();
}

FORCEINLINE void AMRTile::SetTileTransform(const FTransform& NewTransform)
{
	SetActorTransform(NewTransform);
}
