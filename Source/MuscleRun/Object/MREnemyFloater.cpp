// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/MREnemyFloater.h"

// Sets default values
AMREnemyFloater::AMREnemyFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMREnemyFloater::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMREnemyFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMREnemyFloater::MakeDamage_Implementation(float DamageAmount)
{

}

