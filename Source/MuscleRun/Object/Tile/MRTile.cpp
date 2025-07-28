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
	GenerateComponentsFromInfo(ObstacleArray, SpawnedObstacleArray, TEXT("Obstacle"));
	GenerateComponentsFromInfo(PropArray, SpawnedPropArray, TEXT("Prop"));
}

/*
* ����� ���� ������ Ʈ�������� OnConstruction�� ȣ��� �� ������ϴ�.
* �� CallInEditor ���÷��� ������Ƽ�� ������ �Լ��� ��ư�� �����Ϳ� �������,
* ����� ���� ������ ���� ���� �����̳ʰ� ������ �� �ֵ��� ����ϴ�.
*/
void AMRTile::UpdateObstaclesFromComponents()
{
	// Spawned �迭�� ������ �迭�� ������ ������ Ȯ�� (������ġ)
	if (SpawnedObstacleArray.Num() != ObstacleArray.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Obstacle component count and data count do not match. Re-run OnConstruction."));
		return;
	}

	for (int32 i = 0; i < SpawnedObstacleArray.Num(); ++i)
	{
		if (IsValid(SpawnedObstacleArray[i]))
		{
			// ������Ʈ�� ���� Ʈ�������� ������ ������ �迭�� �����մϴ�.
			ObstacleArray[i].ObjectTransform = SpawnedObstacleArray[i]->GetRelativeTransform();
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Obstacle Array data has been updated from components."));
}

/*
* ����� ���� ������ Ʈ�������� OnConstruction�� ȣ��� �� ������ϴ�.
* �� CallInEditor ���÷��� ������Ƽ�� ������ �Լ��� ��ư�� �����Ϳ� �������,
* ����� ���� ������ ���� ���� �����̳ʰ� ������ �� �ֵ��� ����ϴ�.
*/ 
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
	const int32 DataNum = ObjectInfoArray.Num();
	const int32 ComponentNum = SpawnedObjectArray.Num();


	// ��Ұ� �����Ǿ��� ���. � ��Ұ� ������ �� ��, ������Ʈ�� ���� �ı��� �� �����.
	// ��Ұ� �߰��Ǿ��� ����. ��Ұ� �߰��Ǿ� ����ִ� ����, ��� �κп� ���Ե��� �� �� ����.
	// ������ ����ȭ ������ ����ȭ�� ���� ������Ʈ�� ���� �ı��ϰ� �����.
	if (DataNum != ComponentNum)
	{
		// ������ ������ ������Ʈ�� �ı�
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
			const FMRObjectAnchorInfo& ObjectInfo = ObjectInfoArray[i];
			// 1. ������Ʈ ���� (������ �̸� ������ ���� MakeUniqueObjectName ����� ����)
			const FName CompName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), *FString::Printf(TEXT("SpawnedMesh_%s_%d"), *NamePrefix, i));
			TObjectPtr<UStaticMeshComponent> NewMeshComp = NewObject<UStaticMeshComponent>(this, CompName);

			if (ensure(NewMeshComp))
			{
				// 2. ������Ʈ�� Construction Script���� �����Ǿ����� �����Ϳ� �˸�
				NewMeshComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;

				// 3. ������ �ν��Ͻ� ������Ʈ�� �߰�
				AddInstanceComponent(NewMeshComp);

				// 4. ������Ʈ�� �� ���� ������ ����
				NewMeshComp->SetupAttachment(GetRootComponent());

				// 5. ������Ʈ �Ӽ� ����
				// (�߰�) DataNum > ComponentNum�϶��� �� ������Ʈ�� �����մϴ�.
				if (IsValid(ObjectInfo.StaticMesh))
				{
					NewMeshComp->SetRelativeTransform(ObjectInfo.ObjectTransform);
					NewMeshComp->SetStaticMesh(ObjectInfo.StaticMesh);
				}

				// 6. ������Ʈ ���� ��� �� Ȱ��ȭ (���� �߿�)
				NewMeshComp->RegisterComponent();

				SpawnedObjectArray.Add(NewMeshComp);
			}
		}
	}
	// �� ���� �迭 ���ڰ� ���� ����Դϴ�. ���� �� ������ ���� �ʾ� OnConstruction �Լ��� ����ȭ�մϴ�.
	else
	{
		for (int i = 0; i < DataNum; ++i)
		{
			const FMRObjectAnchorInfo& Info = ObjectInfoArray[i];
			UStaticMeshComponent* Comp = SpawnedObjectArray[i];

			if (IsValid(Comp))
			{
				// �����Ϳ� �ٸ� ��쿡�� �Ӽ��� �����Ͽ� ���ʿ��� ������ ���� ���� �ִ�.
				if (Comp->GetStaticMesh() != Info.StaticMesh)
				{
					Comp->SetStaticMesh(Info.StaticMesh);
				}
				if (Comp->GetRelativeTransform().Equals(Info.ObjectTransform) == false)
				{
					Comp->SetRelativeTransform(Info.ObjectTransform);
				}
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
