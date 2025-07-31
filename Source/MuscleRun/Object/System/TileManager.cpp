#include "Object/System/TileManager.h" // ��� ��� ����
#include "Object/Tile/MRTile.h"
#include "Object/Tile/Component/SpawnLocationComponent.h" // [����] �̸� ���� �ݿ�
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

	FTileGroup& CurrentGroup = ActiveTileGroups[CurrentTrackingTileIndex];
	FVector TileEndDirection = [this, CurrentGroup]()->FVector {
		switch (CurrentGroup.ExitDirection)
		{
		case ETrackDirection::East:
			return FVector(0, 1, 0);
		case ETrackDirection::West:
			return FVector(0, -1, 0);
		case ETrackDirection::North:
			return FVector(1, 0, 0);
		case ETrackDirection::South:
			return FVector(-1, 0, 0);
		default:
			return FVector::ZeroVector;
		}
		}();

	if (FVector::DotProduct(PlayerLocation - TileEndLocation, TileEndDirection) > 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("DotProduct Result : %.2f <= %s,  %s"),
		FVector::DotProduct(PlayerLocation - TileEndLocation, TileEndDirection),
			*(PlayerLocation - TileEndLocation).ToString(),
			*TileEndDirection.ToString());
		CurrentTrackingTileIndex++;

		// [����] TArray�� �ε����� ���� ���� Ÿ�Ͽ� �����ϰ� �����մϴ�.
		if (ActiveTileGroups.IsValidIndex(CurrentTrackingTileIndex))
		{
			CurrentTrackingTile = ActiveTileGroups[CurrentTrackingTileIndex].Tile;
			SpawnTile();
			DestroyOldestTileGroup();
		}
	}

	FVector PlaneOrigin = CurrentGroup.Tile->GetEndArrowTransform().GetLocation();
	FVector PlaneNormal = CurrentGroup.Tile->GetEndArrowTransform().GetRotation().GetRightVector();
	FVector VectorToPlayer = PlayerLocation - PlaneOrigin;
	//UE_LOG(LogTemp, Log, TEXT("DotProduct Result In Rotation Logic : %.2f <= %s,  %s"),
		//FVector::DotProduct(VectorToPlayer, PlaneNormal),
		//*(VectorToPlayer).ToString(),
		//*PlaneNormal.ToString());
	if (ensure(ActiveTileGroups.IsValidIndex(CurrentTrackingTileIndex)))
	{
		if (ensure(CurrentGroup.bIsTurnTrigger))
		{
			const float DotProduct = FVector::DotProduct(VectorToPlayer, PlaneNormal);
			//UE_LOG(LogTemp, Log, TEXT("DotProduct Result In Rotation Logic : %.2f"), DotProduct);
			if (DotProduct > 0.f)
			{
				AMRPlayerCharacter* PlayerCharacterRef = Cast<AMRPlayerCharacter>(PlayerCharacter);
				if (ensure(PlayerCharacterRef))
				{
					PlayerCharacterRef->ExecuteForceTurn(CurrentTrackingTile->GetEndArrowTransform(), CurrentGroup.ExitDirection);
					CurrentGroup.bIsTurnTrigger = false;
				}
			}
		}

	}
}

void ATileManager::SpawnTile()
{
	if (TileClassMap.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ATileManager: TileClass is not set!"));
		return;
	}

	ETileType TypeToSpawn = ETileType::TurnLeft; // ���� ���� ���� DecideNextTileType()
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

			// [����] TArray�� �� �ڿ� �� �׷��� �߰��մϴ�.
			ActiveTileGroups.Add(NewGroup);

			NextSpawnPointTransform = NewTile->GetEndArrowTransform();
		}
	}
}

void ATileManager::DestroyOldestTileGroup()
{
	// [����] TArray�� Num() �Լ��� ����մϴ�.
	if (ActiveTileGroups.Num() > MinActiveTiles)
	{
		// [����] TArray�� ù ��° ��Ҹ� �������� �����մϴ�.
		FTileGroup OldestGroup = ActiveTileGroups[0];
		ActiveTileGroups.RemoveAt(0);

		// [����] �ε����� ���������Ƿ�, ���� ���� �ε����� �ϳ� �ٿ��� �迭 ������ ����ϴ�.
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

	// [����] �̸� ���� �ݿ�
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
			//UE_LOG(LogTemp, Log, TEXT("SpawnedComponentTransform : %s, %s"), *Point->GetComponentTransform().ToString(), *Point->GetFName().ToString());
			if (SpawnedObject)
			{
				OutSpawnedActors.Add(SpawnedObject);
			}
		}
	}
}
