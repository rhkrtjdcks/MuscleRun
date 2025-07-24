// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/MRObsrtuctBase.h"

// Sets default values
AMRObsrtuctBase::AMRObsrtuctBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMRObsrtuctBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMRObsrtuctBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMRObsrtuctBase::MakeDamage_Implementation(float DamageAmount)
{

}

