// -----------------------------------------------------------------------------
// AMRTile.cpp - 최종 수정본
// -----------------------------------------------------------------------------
#include "Object/Tile/MRTile.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

AMRTile::AMRTile()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp_TileEntire"));
	MeshComp->SetupAttachment(RootComponent);

	StartArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("StartArrow"));
	StartArrowComponent->SetupAttachment(RootComponent);

	EndArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("EndArrow"));
	EndArrowComponent->SetupAttachment(RootComponent);
}

void AMRTile::BeginPlay()
{
	Super::BeginPlay();

	// 게임이 시작되면, 에디터에서만 보이던 장애물 '미리보기' 컴포넌트들은 숨깁니다.
	// 실제 게임플레이용 장애물 액터는 ATileManager가 스폰할 것이기 때문입니다.
	// Prop 컴포넌트는 실제 장식이므로 그대로 유지합니다.
	for (auto& Pair : IDToComponentObstacleMap)
	{
		if (Pair.Value)
		{
			Pair.Value->SetVisibility(false);
			Pair.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void AMRTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 에디터에서 타일의 속성이 변경될 때마다 시각적 표현을 업데이트합니다.
	UpdateObstaclePreviews();
	UpdatePropComponents();
}

void AMRTile::SaveGizmoChangeForObstacle()
{
	for (const auto& Pair : IDToComponentObstacleMap)
	{
		if (!IsValid(Pair.Value)) continue;

		for (FMRObstacleSpawnInfo& Info : ObstacleArray)
		{
			if (Info.ObjectID == Pair.Key)
			{
				Info.ObjectTransform = Pair.Value->GetRelativeTransform();
				break;
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Obstacle gizmo changes saved."));
}

void AMRTile::SaveGizmoChangeForProp()
{
	for (const auto& Pair : IDToComponentPropMap)
	{
		if (!IsValid(Pair.Value)) continue;

		for (FMRPropInfo& Info : PropArray)
		{
			if (Info.ObjectID == Pair.Key)
			{
				Info.ObjectTransform = Pair.Value->GetRelativeTransform();
				break;
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Prop gizmo changes saved."));
}

void AMRTile::UpdateObstaclePreviews()
{
	TSet<FGuid> CurrentDataIDs;
	for (const auto& Info : ObstacleArray)
	{
		CurrentDataIDs.Add(Info.ObjectID);
	}

	TArray<FGuid> ObsoleteIDs;
	for (const auto& Pair : IDToComponentObstacleMap)
	{
		if (!CurrentDataIDs.Contains(Pair.Key))
		{
			if (IsValid(Pair.Value))
			{
				Pair.Value->DestroyComponent();
			}
			ObsoleteIDs.Add(Pair.Key);
		}
	}
	for (const FGuid& ID : ObsoleteIDs)
	{
		IDToComponentObstacleMap.Remove(ID);
	}

	for (const auto& Info : ObstacleArray)
	{
		TObjectPtr<UStaticMeshComponent> ComponentToUpdate = nullptr;
		if (TObjectPtr<UStaticMeshComponent>* ExistingCompPtr = IDToComponentObstacleMap.Find(Info.ObjectID))
		{
			ComponentToUpdate = *ExistingCompPtr;
		}
		else
		{
			const FName CompName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), *FString::Printf(TEXT("ObstaclePreview_%s"), *Info.ObjectID.ToString()));
			ComponentToUpdate = NewObject<UStaticMeshComponent>(this, CompName);

			ComponentToUpdate->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			ComponentToUpdate->bEditableWhenInherited = true;

			AddInstanceComponent(ComponentToUpdate);
			ComponentToUpdate->SetupAttachment(RootComponent);
			ComponentToUpdate->RegisterComponent();
			IDToComponentObstacleMap.Add(Info.ObjectID, ComponentToUpdate);
		}

		if (ComponentToUpdate)
		{
			ComponentToUpdate->SetStaticMesh(Info.PreviewMesh);
			ComponentToUpdate->SetRelativeTransform(Info.ObjectTransform);
		}
	}
}

void AMRTile::UpdatePropComponents()
{
	TSet<FGuid> CurrentDataIDs;
	for (const auto& Info : PropArray)
	{
		CurrentDataIDs.Add(Info.ObjectID);
	}

	TArray<FGuid> ObsoleteIDs;
	for (const auto& Pair : IDToComponentPropMap)
	{
		if (!CurrentDataIDs.Contains(Pair.Key))
		{
			if (IsValid(Pair.Value))
			{
				Pair.Value->DestroyComponent();
			}
			ObsoleteIDs.Add(Pair.Key);
		}
	}
	for (const FGuid& ID : ObsoleteIDs)
	{
		IDToComponentPropMap.Remove(ID);
	}

	for (const auto& Info : PropArray)
	{
		TObjectPtr<UStaticMeshComponent> ComponentToUpdate = nullptr;
		if (TObjectPtr<UStaticMeshComponent>* ExistingCompPtr = IDToComponentPropMap.Find(Info.ObjectID))
		{
			ComponentToUpdate = *ExistingCompPtr;
		}
		else
		{
			const FName CompName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), *FString::Printf(TEXT("Prop_%s"), *Info.ObjectID.ToString()));
			ComponentToUpdate = NewObject<UStaticMeshComponent>(this, CompName);

			ComponentToUpdate->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			ComponentToUpdate->bEditableWhenInherited = true;

			AddInstanceComponent(ComponentToUpdate);
			ComponentToUpdate->SetupAttachment(RootComponent);
			ComponentToUpdate->RegisterComponent();
			IDToComponentPropMap.Add(Info.ObjectID, ComponentToUpdate);
		}

		if (ComponentToUpdate)
		{
			ComponentToUpdate->SetStaticMesh(Info.StaticMesh);
			ComponentToUpdate->SetRelativeTransform(Info.ObjectTransform);
		}
	}
}

FTransform AMRTile::GetEndArrowTransform() const
{
	return EndArrowComponent->GetComponentTransform();
}
