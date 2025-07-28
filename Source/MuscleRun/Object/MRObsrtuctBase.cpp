// Fill out your copyright notice in the Description page of Project Settings.

#include "MRObsrtuctBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

AMRObsrtuctBase::AMRObsrtuctBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}