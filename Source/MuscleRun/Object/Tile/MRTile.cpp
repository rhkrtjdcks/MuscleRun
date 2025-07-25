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
	// 기존에 생성된 컴포넌트들 파괴
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
		if (IsValid(ObjectInfo.StaticMesh))
		{
			// 1. 컴포넌트 생성 (고유한 이름 보장을 위해 MakeUniqueObjectName 사용을 권장)
			const FName CompName = MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), *FString::Printf(TEXT("SpawnedMesh_%s_%d"), *NamePrefix, i));
			TObjectPtr<UStaticMeshComponent> NewMeshComp = NewObject<UStaticMeshComponent>(this, CompName);

			if (ensure(NewMeshComp))
			{
				// 2. 컴포넌트가 Construction Script에서 생성되었음을 에디터에 알림
				NewMeshComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;

				// 3. 액터의 인스턴스 컴포넌트로 추가
				AddInstanceComponent(NewMeshComp);

				// 4. 컴포넌트를 씬 계층 구조에 부착
				NewMeshComp->SetupAttachment(GetRootComponent());

				// 5. 컴포넌트 속성 설정
				NewMeshComp->SetRelativeTransform(ObjectInfo.ObjectTransform);
				NewMeshComp->SetStaticMesh(ObjectInfo.StaticMesh);

				// 6. 컴포넌트 최종 등록 및 활성화 (가장 중요)
				NewMeshComp->RegisterComponent();

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
