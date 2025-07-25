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
	// 시작 위치에 위치할 StartArrowComponent를 ArrowComponent에 붙입니다.
	TriggerVolume->SetupAttachment(StartArrowComponent);
}

// Called when the game starts or when spawned
void AMRTile::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AMRTile::OnOverlapBegin);

	// 생성 시 오브젝트에서 해야 할 효과들이 있다면(Cpp 로직이 정말로 필요하다면) 추후 여기서 지정합니다.
	// 왠만해서는 오브젝트 레벨(머테리얼)에서 지정해주세요.
}

void AMRTile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnTileGeneratedOvelaped.Broadcast(this);
}

void AMRTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 에디터가 변경될 경우 다음 함수들을 실행해서 생성한 오브젝트들을 관리합니다.
	UpdateComponentsFromInfo(ObstacleArray, IDToComponentObstacleMap, TEXT("Obstacle"));
	UpdateComponentsFromInfo(PropArray, IDToComponentPropMap, TEXT("Prop"));
}

/*
* 기즈모를 통해 변경한 트랜스폼은 OnConstruction이 호출될 때 사라집니다.
* 이 CallInEditor 리플렉션 프로퍼티로 선언한 함수로 버튼을 에디터에 노출시켜,
* 기즈모를 통해 변경한 값을 쉽게 디자이너가 적용할 수 있도록 만듭니다.
*/
void AMRTile::SaveGizmoChangeForObstacle()
{
	// 컴포넌트 맵을 순회하여, 현재 컴포넌트의 상태를 데이터 배열에 덮어쓴다.
	for (const auto& Pair : IDToComponentObstacleMap)
	{
		const FGuid& ObjectID = Pair.Key;
		const UStaticMeshComponent* Comp = Pair.Value;

		if (IsValid(Comp))
		{
			// ObstacleArray에서 이 ID를 가진 Info를 찾는다.
			// 이 과정은 배열이 크면 비효율적일 수 있지만, CallInEditor 함수에서는 괜찮다.
			for (FMRObjectAnchorInfo& Info : ObstacleArray)
			{
				if (Info.ObjectID == ObjectID)
				{
					// 찾았다면, 컴포넌트의 현재 트랜스폼을 데이터에 저장한다.
					Info.ObjectTransform = Comp->GetRelativeTransform();
					break; // 해당 ID에 대한 작업이 끝났으므로 안쪽 루프 탈출
				}
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Gizmo changes for obstacles have been saved to the data array."));
}

/*
* 기즈모를 통해 변경한 트랜스폼은 OnConstruction이 호출될 때 사라집니다.
* 이 CallInEditor 리플렉션 프로퍼티로 선언한 함수로 버튼을 에디터에 노출시켜,
* 기즈모를 통해 변경한 값을 쉽게 디자이너가 적용할 수 있도록 만듭니다.
*/ 
void AMRTile::SaveGizmoChangeForProp()
{
	// 컴포넌트 맵을 순회하여, 현재 컴포넌트의 상태를 데이터 배열에 덮어쓴다.
	for (const auto& Pair : IDToComponentPropMap)
	{
		const FGuid& ObjectID = Pair.Key;
		const UStaticMeshComponent* Comp = Pair.Value;

		if (IsValid(Comp))
		{
			// ObstacleArray에서 이 ID를 가진 Info를 찾는다.
			// 이 과정은 배열이 크면 비효율적일 수 있지만, CallInEditor 함수에서는 괜찮다.
			for (FMRObjectAnchorInfo& Info : PropArray)
			{
				if (Info.ObjectID == ObjectID)
				{
					// 찾았다면, 컴포넌트의 현재 트랜스폼을 데이터에 저장한다.
					Info.ObjectTransform = Comp->GetRelativeTransform();
					break; // 해당 ID에 대한 작업이 끝났으므로 안쪽 루프 탈출
				}
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Gizmo changes for props have been saved to the data array."));
}

void AMRTile::UpdateComponentsFromInfo(const TArray<FMRObjectAnchorInfo>& ObjectInfoArray, TMap<FGuid, TObjectPtr<UStaticMeshComponent>>& IDToComponentMap, const FString& NamePrefix)
{
	// 1단계: 현재 데이터에 있는 ID들을 모두 TSet에 모은다. (빠른 조회를 위함)
	TSet<FGuid> CurrentDataIDs;
	for (const auto& Info : ObjectInfoArray)
	{
		CurrentDataIDs.Add(Info.ObjectID);
	}

	// 2단계: 현재 생성된 컴포넌트들(IDToComponentMap)을 순회하며,
	// 더 이상 데이터에 존재하지 않는 ID를 가진 컴포넌트는 파괴한다.
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
	// 맵에서 파괴된 컴포넌트 정보 제거
	for (const FGuid& ID : ObsoleteIDs)
	{
		IDToComponentMap.Remove(ID);
	}

	// 3단계: 데이터 배열을 순회하며, 각 데이터를 기준으로 컴포넌트를 업데이트하거나 생성한다.
	for (const auto& Info : ObjectInfoArray)
	{
		// 이 ID를 가진 컴포넌트가 이미 맵에 존재하는가?
		if (TObjectPtr<UStaticMeshComponent>* ExistingCompPtr = IDToComponentMap.Find(Info.ObjectID))
		{
			// [업데이트]
			// 이미 존재한다! 파괴하지 않고 속성만 업데이트한다.
			// 이러면 머티리얼 같은 다른 설정이 그대로 보존된다.
			UStaticMeshComponent* Comp = *ExistingCompPtr;
			if (IsValid(Comp))
			{
				Comp->SetStaticMesh(Info.StaticMesh);
				Comp->SetRelativeTransform(Info.ObjectTransform);
			}
		}
		else
		{

			

			// [생성]
			// 존재하지 않는다! 이것은 새로 추가된 데이터다.
			// 새로운 컴포넌트를 생성하고 맵에 추가한다.
			// (이전에 만들었던 컴포넌트 생성 로직 사용)

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
