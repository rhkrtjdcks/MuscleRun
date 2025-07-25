// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Tile/MRTile.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
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

	StartArrowComponent = CreateDefaultSubobject<UArrowComponent>("StartArrow");
	EndArrowComponent = CreateDefaultSubobject<UArrowComponent>("EndArrow");

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>("TriggerVolume");
	// ���� ��ġ�� ��ġ�� StartArrowComponent�� ArrowComponent�� ���Դϴ�.
	TriggerVolume->SetupAttachment(StartArrowComponent);
}

// Called when the game starts or when spawned
void AMRTile::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AMRTile::OnOverlapBegin);

	// ���� �� ������Ʈ���� �ؾ� �� ȿ������ �ִٸ�(Cpp ������ ������ �ʿ��ϴٸ�) ���� ���⼭ �����մϴ�.
	// �ظ��ؼ��� ������Ʈ ����(���׸���)���� �������ּ���.
}

void AMRTile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnTileGeneratedOvelaped.Broadcast(this);
}

void AMRTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// �����Ͱ� ����� ��� ���� �Լ����� �����ؼ� ������ ������Ʈ���� �����մϴ�.
	UpdateComponentsFromInfo(ObstacleArray, IDToComponentObstacleMap, TEXT("Obstacle"));
	UpdateComponentsFromInfo(PropArray, IDToComponentPropMap, TEXT("Prop"));
}

/*
* ����� ���� ������ Ʈ�������� OnConstruction�� ȣ��� �� ������ϴ�.
* �� CallInEditor ���÷��� ������Ƽ�� ������ �Լ��� ��ư�� �����Ϳ� �������,
* ����� ���� ������ ���� ���� �����̳ʰ� ������ �� �ֵ��� ����ϴ�.
*/
void AMRTile::SaveGizmoChangeForObstacle()
{
	// ������Ʈ ���� ��ȸ�Ͽ�, ���� ������Ʈ�� ���¸� ������ �迭�� �����.
	for (const auto& Pair : IDToComponentObstacleMap)
	{
		const FGuid& ObjectID = Pair.Key;
		const UStaticMeshComponent* Comp = Pair.Value;

		if (IsValid(Comp))
		{
			// ObstacleArray���� �� ID�� ���� Info�� ã�´�.
			// �� ������ �迭�� ũ�� ��ȿ������ �� ������, CallInEditor �Լ������� ������.
			for (FMRObjectAnchorInfo& Info : ObstacleArray)
			{
				if (Info.ObjectID == ObjectID)
				{
					// ã�Ҵٸ�, ������Ʈ�� ���� Ʈ�������� �����Ϳ� �����Ѵ�.
					Info.ObjectTransform = Comp->GetRelativeTransform();
					break; // �ش� ID�� ���� �۾��� �������Ƿ� ���� ���� Ż��
				}
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Gizmo changes for obstacles have been saved to the data array."));
}

/*
* ����� ���� ������ Ʈ�������� OnConstruction�� ȣ��� �� ������ϴ�.
* �� CallInEditor ���÷��� ������Ƽ�� ������ �Լ��� ��ư�� �����Ϳ� �������,
* ����� ���� ������ ���� ���� �����̳ʰ� ������ �� �ֵ��� ����ϴ�.
*/ 
void AMRTile::SaveGizmoChangeForProp()
{
	// ������Ʈ ���� ��ȸ�Ͽ�, ���� ������Ʈ�� ���¸� ������ �迭�� �����.
	for (const auto& Pair : IDToComponentPropMap)
	{
		const FGuid& ObjectID = Pair.Key;
		const UStaticMeshComponent* Comp = Pair.Value;

		if (IsValid(Comp))
		{
			// ObstacleArray���� �� ID�� ���� Info�� ã�´�.
			// �� ������ �迭�� ũ�� ��ȿ������ �� ������, CallInEditor �Լ������� ������.
			for (FMRObjectAnchorInfo& Info : PropArray)
			{
				if (Info.ObjectID == ObjectID)
				{
					// ã�Ҵٸ�, ������Ʈ�� ���� Ʈ�������� �����Ϳ� �����Ѵ�.
					Info.ObjectTransform = Comp->GetRelativeTransform();
					break; // �ش� ID�� ���� �۾��� �������Ƿ� ���� ���� Ż��
				}
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Gizmo changes for props have been saved to the data array."));
}

void AMRTile::UpdateComponentsFromInfo(const TArray<FMRObjectAnchorInfo>& ObjectInfoArray, TMap<FGuid, TObjectPtr<UStaticMeshComponent>>& IDToComponentMap, const FString& NamePrefix)
{
	// 1�ܰ�: ���� �����Ϳ� �ִ� ID���� ��� TSet�� ������. (���� ��ȸ�� ����)
	TSet<FGuid> CurrentDataIDs;
	for (const auto& Info : ObjectInfoArray)
	{
		CurrentDataIDs.Add(Info.ObjectID);
	}

	// 2�ܰ�: ���� ������ ������Ʈ��(IDToComponentMap)�� ��ȸ�ϸ�,
	// �� �̻� �����Ϳ� �������� �ʴ� ID�� ���� ������Ʈ�� �ı��Ѵ�.
	TArray<FGuid> ObsoleteIDs;
	for (const auto& Pair : IDToComponentMap)
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
	// �ʿ��� �ı��� ������Ʈ ���� ����
	for (const FGuid& ID : ObsoleteIDs)
	{
		IDToComponentMap.Remove(ID);
	}

	// 3�ܰ�: ������ �迭�� ��ȸ�ϸ�, �� �����͸� �������� ������Ʈ�� ������Ʈ�ϰų� �����Ѵ�.
	for (const auto& Info : ObjectInfoArray)
	{
		// �� ID�� ���� ������Ʈ�� �̹� �ʿ� �����ϴ°�?
		if (TObjectPtr<UStaticMeshComponent>* ExistingCompPtr = IDToComponentMap.Find(Info.ObjectID))
		{
			// [������Ʈ]
			// �̹� �����Ѵ�! �ı����� �ʰ� �Ӽ��� ������Ʈ�Ѵ�.
			// �̷��� ��Ƽ���� ���� �ٸ� ������ �״�� �����ȴ�.
			UStaticMeshComponent* Comp = *ExistingCompPtr;
			if (IsValid(Comp))
			{
				Comp->SetStaticMesh(Info.StaticMesh);
				Comp->SetRelativeTransform(Info.ObjectTransform);
			}
		}
		else
		{

			

			// [����]
			// �������� �ʴ´�! �̰��� ���� �߰��� �����ʹ�.
			// ���ο� ������Ʈ�� �����ϰ� �ʿ� �߰��Ѵ�.
			// (������ ������� ������Ʈ ���� ���� ���)

			const FName CompName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), *FString::Printf(TEXT("SpawnedMesh_%s_%s"), *NamePrefix, *Info.ObjectID.ToString()));
			TObjectPtr<UStaticMeshComponent> NewMeshComp = NewObject<UStaticMeshComponent>(this, CompName);


			NewMeshComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			AddInstanceComponent(NewMeshComp);
			NewMeshComp->SetupAttachment(GetRootComponent());
			NewMeshComp->SetStaticMesh(Info.StaticMesh);
			NewMeshComp->SetRelativeTransform(Info.ObjectTransform);
			NewMeshComp->RegisterComponent();
			IDToComponentMap.Add(Info.ObjectID, NewMeshComp);
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
