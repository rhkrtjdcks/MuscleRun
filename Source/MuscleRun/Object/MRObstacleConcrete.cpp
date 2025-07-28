// Fill out your copyright notice in the Description page of Project Settings.

#include "MRObstacleConcrete.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

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

	// ✨ OnHit 이벤트가 플레이어와 정상적으로 발생하도록 콜리전 설정을 수정합니다.
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	MeshComponent->SetNotifyRigidBodyCollision(true);
}

void AMRObstacleConcrete::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	ProjectileMovementComponent->Velocity = FVector(1.0f, 0.0f, 0.0f) * ProjectileMovementComponent->InitialSpeed;

	// ✨ OnHit 함수를 이벤트에 연결합니다.
	MeshComponent->OnComponentHit.AddDynamic(this, &AMRObstacleConcrete::OnHit);
}

void AMRObstacleConcrete::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 거리 기반의 방향 전환 로직
	const float DistanceMoved = FVector::Dist(StartLocation, GetActorLocation());
	if (MoveDistance > 0.f && DistanceMoved > MoveDistance)
	{
		ProjectileMovementComponent->Velocity *= -1;
		StartLocation = GetActorLocation();
	}
}

// AMRObstacleConcrete.cpp

void AMRObstacleConcrete::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	// 부딪힌 대상이 플레이어 캐릭터인지 확인
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// 1. 데미지 적용
		UGameplayStatics::ApplyDamage(PlayerCharacter, Damage, UGameplayStatics::GetPlayerController(this, 0), this, UDamageType::StaticClass());

		// ✨ 2. 넉백 방향을 충돌 지점의 법선 벡터(ImpactNormal)로 계산합니다.
		FVector KnockbackDirection = -Hit.ImpactNormal; //  -넣어서 자석처럼 빨려들어가는 느낌제거
		KnockbackDirection.Z = 0.5f; // 살짝 위로 튕겨내도록 Z값 추가
		KnockbackDirection.Normalize();

		// 3. 플레이어에게 넉백을 적용합니다.
		PlayerCharacter->LaunchCharacter(KnockbackDirection * KnockbackStrength, true, true);

		// 장애물 자신의 이동 로직은 건드리지 않습니다.
	}
	else
	{
		// 대상이 플레이어가 아닐 경우 (예: 벽)
		StartLocation = GetActorLocation();
	}
}