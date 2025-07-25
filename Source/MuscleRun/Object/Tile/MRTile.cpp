// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Tile/MRTile.h"

// Sets default values
AMRTile::AMRTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMRTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMRTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMRTile::GetTileTransform()
{

}

void AMRTile::SetTileTransform()
{

}

void AMRTile::OnOverlapBeginned()
{

}

