#include "Object/System/TileManager.h" // 헤더 경로 수정
#include "Object/Tile/MRTile.h"
#include "Object/Tile/Component/SpawnLocationComponent.h" // [수정] 이름 변경 반영
#include "GameFramework/Character.h"
#include "Object/Tile/DataAsset/DA_SpawnableObjects.h"
#include "Kismet/GameplayStatics.h"
#include "Sys/GameState/MRGameState.h"
#include "Character/MRPlayerCharacter.h"

ATileManager::ATileManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATileManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("ATileManager: PlayerCharacter not found!"));
		SetActorTickEnabled(false);
		return;
	}

	NextSpawnPointTransform = GetActorTransform();

	for (int i = 0; i < NumInitialTiles; ++i)
	{
		SpawnTile();
	}

	if (ActiveTileGroups.Num() > 0)
	{
		CurrentTrackingTile = ActiveTileGroups[0].Tile;
		CurrentTrackingTileIndex = 0;
	}

	if (AMRGameState* GS = GetWorld()->GetGameState<AMRGameState>())
	{
		GS->RegisterTileManager(this);
	}
}

void ATileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerCharacter || !CurrentTrackingTile) return;

	FVector TileEndLocation = CurrentTrackingTile->GetEndArrowTransform().GetLocation();
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	FVector TileForwardVector = CurrentTrackingTile->GetActorForwardVector();

	if (FVector::DotProduct(PlayerLocation - TileEndLocation, TileForwardVector) > 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("DotProduct Result : %.2f <= %s,  %s"),
		FVector::DotProduct(PlayerLocation - TileEndLocation, TileForwardVector),
			*(PlayerLocation - TileEndLocation).ToString(),
			*TileForwardVector.ToString());
		CurrentTrackingTileIndex++;

		// [수정] TArray의 인덱스로 다음 추적 타일에 안전하게 접근합니다.
		if (ActiveTileGroups.IsValidIndex(CurrentTrackingTileIndex))
		{
			CurrentTrackingTile = ActiveTileGroups[CurrentTrackingTileIndex].Tile;
			SpawnTile();
			DestroyOldestTileGroup();
		}
	}
	if (ActiveTileGroups.IsValidIndex(CurrentTrackingTileIndex))
	{
		FTileGroup& CurrentGroup = ActiveTileGroups[CurrentTrackingTileIndex];
		if (CurrentGroup.bIsTurnTrigger)
		{
			FVector PlaneOrigin = CurrentGroup.Tile->GetEndArrowTransform().GetLocation();
			FVector PlaneNormal = CurrentGroup.Tile->GetEndArrowTransform().GetRotation().GetForwardVector();
			FVector PlayerLocation = PlayerCharacter->GetActorLocation();
			FVector VectorToPlayer = PlayerLocation - PlaneOrigin;

			const float DotProduct = FVector::DotProduct(PlaneNormal, VectorToPlayer);
			if (DotProduct > 0.f)
			{
				AMRPlayerCharacter* PlayerCharacterRef = Cast<AMRPlayerCharacter>(PlayerCharacter);
				if (PlayerCharacterRef)
				{
					// PlayerCharacterRef->ExecuteForceTurn(PlaneOrigin, CurrentGroup.ExitDirection);
					bIsTrunTrigger = false;
				}
			}
		}

	}
}

void ATileManager::SpawnTile()
{
	if (!TileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATileManager: TileClass is not set!"));
		return;
	}

	ETileType TypeToSpawn = ETileType::Straight; // 추후 로직 구현 DecideNextTileType()
	TSubclassOf<AMRTile>* FoundClassPtr = TileClassMap.Find(TypeToSpawn);

	if (ensure(FoundClassPtr && *FoundClassPtr))
	{
		TSubclassOf<AMRTile> ClassToSpawn = *FoundClassPtr;
		AMRTile* NewTile = GetWorld()->SpawnActor<AMRTile>(ClassToSpawn, NextSpawnPointTransform);
		if (ensure(NewTile))
		{
			FTileGroup NewGroup;
			NewGroup.Tile = NewTile;

			if (TypeToSpawn != ETileType::Straight)
			{
				int32 CastedType = static_cast<int32>(LastTileExitDirection);
				switch (TypeToSpawn)
				{
				case ETileType::TurnLeft:
					CastedType -= 1;
					break;
				case ETileType::TurnRight:
					CastedType += 1;
					break;
				default:
					ensureMsgf(false, TEXT("CalculateNextDirection received an unexpected TileType!"));
					break;
				}
				LastTileExitDirection = static_cast<ETrackDirection>((CastedType + 4) % 4);

				NewGroup.bIsTurnTrigger = true;
			}
			NewGroup.ExitDirection = LastTileExitDirection;
			//

			SpawnObjectsOnTile(NewTile, NewGroup.ContainedActors);

			// [수정] TArray의 맨 뒤에 새 그룹을 추가합니다.
			ActiveTileGroups.Add(NewGroup);

			NextSpawnPointTransform = NewTile->GetEndArrowTransform();
		}
	}
}

void ATileManager::DestroyOldestTileGroup()
{
	// [수정] TArray의 Num() 함수를 사용합니다.
	if (ActiveTileGroups.Num() > MinActiveTiles)
	{
		// [수정] TArray의 첫 번째 요소를 가져오고 제거합니다.
		FTileGroup OldestGroup = ActiveTileGroups[0];
		ActiveTileGroups.RemoveAt(0);

		// [수정] 인덱스를 제거했으므로, 현재 추적 인덱스도 하나 줄여서 배열 범위를 맞춥니다.
		CurrentTrackingTileIndex--;

		for (TObjectPtr<AActor> ActorToDestroy : OldestGroup.ContainedActors)
		{
			if (IsValid(ActorToDestroy))
			{
				ActorToDestroy->Destroy();
			}
		}

		if (IsValid(OldestGroup.Tile))
		{
			OldestGroup.Tile->Destroy();
		}
	}
}

void ATileManager::SpawnObjectsOnTile(AMRTile* TargetTile, TArray<TObjectPtr<AActor>>& OutSpawnedActors)
{
	if (!TargetTile) return;

	// [수정] 이름 변경 반영
	TArray<USpawnLocationComponent*> SpawnPoints;
	TargetTile->GetComponents<USpawnLocationComponent>(SpawnPoints);

	for (USpawnLocationComponent* Point : SpawnPoints)
	{
		if (Point && Point->ActorGroupToSpawn && Point->ActorGroupToSpawn->ActorClasses.Num() > 0)
		{
			const TArray<TSubclassOf<AActor>>& SpawnableClasses = Point->ActorGroupToSpawn->ActorClasses;
			TSubclassOf<AActor> ClassToSpawn = SpawnableClasses[FMath::RandRange(0, SpawnableClasses.Num() - 1)];

			FTransform SpawnTransform = Point->GetComponentTransform();
			AActor* SpawnedObject = GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTransform);
			UE_LOG(LogTemp, Log, TEXT("SpawnedComponentTransform : %s, %s"), *Point->GetComponentTransform().ToString(), *Point->GetFName().ToString());
			if (SpawnedObject)
			{
				OutSpawnedActors.Add(SpawnedObject);
			}
		}
	}
}
