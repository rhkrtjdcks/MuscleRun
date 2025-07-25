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
	GenerateComponentsFromInfo(ObstacleArray, SpawnedObstacleArray, TEXT("Obstacle"));
	GenerateComponentsFromInfo(PropArray, SpawnedPropArray, TEXT("Prop"));
}

/*
* 기즈모를 통해 변경한 트랜스폼은 OnConstruction이 호출될 때 사라집니다.
* 이 CallInEditor 리플렉션 프로퍼티로 선언한 함수로 버튼을 에디터에 노출시켜,
* 기즈모를 통해 변경한 값을 쉽게 디자이너가 적용할 수 있도록 만듭니다.
*/
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

/*
* 기즈모를 통해 변경한 트랜스폼은 OnConstruction이 호출될 때 사라집니다.
* 이 CallInEditor 리플렉션 프로퍼티로 선언한 함수로 버튼을 에디터에 노출시켜,
* 기즈모를 통해 변경한 값을 쉽게 디자이너가 적용할 수 있도록 만듭니다.
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


	// 요소가 삭제되었을 경우. 어떤 요소가 삭제된 지 모름, 컴포넌트를 전부 파괴한 후 재생성.
	// 요소가 추가되었을 직후. 요소가 추가되어 비어있는 상태, 어느 부분에 삽입된지 알 수 없다.
	// 추후의 동기화 로직의 간편화를 위해 컴포넌트를 전부 파괴하고 재생성.
	if (DataNum != ComponentNum)
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
				// (추가) DataNum > ComponentNum일때는 빈 컴포넌트를 생성합니다.
				if (IsValid(ObjectInfo.StaticMesh))
				{
					NewMeshComp->SetRelativeTransform(ObjectInfo.ObjectTransform);
					NewMeshComp->SetStaticMesh(ObjectInfo.StaticMesh);
				}

				// 6. 컴포넌트 최종 등록 및 활성화 (가장 중요)
				NewMeshComp->RegisterComponent();

				SpawnedObjectArray.Add(NewMeshComp);
			}
		}
	}
	// 두 개의 배열 숫자가 같을 경우입니다. 삭제 및 생성을 하지 않아 OnConstruction 함수를 최적화합니다.
	else
	{
		for (int i = 0; i < DataNum; ++i)
		{
			const FMRObjectAnchorInfo& Info = ObjectInfoArray[i];
			UStaticMeshComponent* Comp = SpawnedObjectArray[i];

			if (IsValid(Comp))
			{
				// 데이터와 다른 경우에만 속성을 설정하여 불필요한 연산을 줄일 수도 있다.
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
