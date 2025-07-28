﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "MRObstacleConcrete.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"

AMRObstacleConcrete::AMRObstacleConcrete()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = MeshComponent;
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 1.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
}

void AMRObstacleConcrete::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	ProjectileMovementComponent->Velocity = FVector(1.0f, 0.0f, 0.0f) * ProjectileMovementComponent->InitialSpeed;
}

void AMRObstacleConcrete::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float DistanceMoved = FVector::Dist(StartLocation, GetActorLocation());
	if (MoveDistance > 0.f && DistanceMoved > MoveDistance)
	{
		ProjectileMovementComponent->Velocity *= -1;
		StartLocation = GetActorLocation();
	}
}

void AMRObstacleConcrete::OnObstacleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// ✨ 1. 부모 클래스의 OnObstacleHit 기능을 먼저 실행합니다 (플레이어에게 데미지/넉백).
	Super::OnObstacleHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	// ✨ 2. 부딪힌 대상이 플레이어가 아닐 경우에만 이 장애물 고유의 로직을 실행합니다.
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter == nullptr)
	{
		// 대상이 플레이어가 아니면 (예: 벽), 왕복 운동을 위해 시작 위치를 리셋합니다.
		StartLocation = GetActorLocation();
	}
}