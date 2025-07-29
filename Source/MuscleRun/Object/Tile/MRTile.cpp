// -----------------------------------------------------------------------------
// AMRTile.cpp - ���� ������
// -----------------------------------------------------------------------------
#include "Object/Tile/MRTile.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

AMRTile::AMRTile()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshCompTileEntire"));
	MeshComp->SetupAttachment(RootComponent);

	StartArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("StartArrow"));
	StartArrowComponent->SetupAttachment(RootComponent);

	EndArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("EndArrow"));
	EndArrowComponent->SetupAttachment(RootComponent);
}

void AMRTile::BeginPlay()
{
	Super::BeginPlay();

	// ������ ���۵Ǹ�, �����Ϳ����� ���̴� ��ֹ� '�̸�����' ������Ʈ���� ����ϴ�.
	// ���� �����÷��̿� ��ֹ� ���ʹ� ATileManager�� ������ ���̱� �����Դϴ�.
	// Prop ������Ʈ�� ���� ����̹Ƿ� �״�� �����մϴ�.
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

	// �����Ϳ��� Ÿ���� �Ӽ��� ����� ������ �ð��� ǥ���� ������Ʈ�մϴ�.
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
		

		for (int32 i = 0; i < ObstacleArray.Num(); ++i)
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
	// UE_LOG(LogTemp, Log, TEXT("CurrentComponentTransform : %s, %s"), *EndArrowComponent->GetComponentTransform().ToString(), *GetFName().ToString());
	return EndArrowComponent->GetComponentTransform();
}
