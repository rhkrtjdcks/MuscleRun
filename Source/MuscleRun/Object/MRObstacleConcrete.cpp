// Fill out your copyright notice in the Description page of Project Settings.

#include "MRObstacleConcrete.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"

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

	// ✨ 1. 자동 회전 기능을 비활성화합니다.
	ProjectileMovementComponent->bRotationFollowsVelocity = false;

	// 부모 클래스에서 데미지 값을 설정할 수 있습니다.
	Damage = 25.0f;

	// ✨ 2. OnHit 함수를 사용하기 위해 아래 두 줄을 추가합니다.
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->OnComponentHit.AddDynamic(this, &AMRObstacleConcrete::OnHit);
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

	// 거리 기반의 방향 전환 로직
	const float DistanceMoved = FVector::Dist(StartLocation, GetActorLocation());
	if (DistanceMoved > MoveDistance)
	{
		ProjectileMovementComponent->Velocity *= -1;
		StartLocation = GetActorLocation();
	}
}

void AMRObstacleConcrete::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 벽에 부딪혔을 때 시작 위치를 리셋하는 로직
	if (OtherActor && (OtherActor != this))
	{
		StartLocation = GetActorLocation();
	}
}