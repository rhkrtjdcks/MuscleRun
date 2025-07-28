// Fill out your copyright notice in the Description page of Project Settings.

#include "MRObsrtuctBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

AMRObsrtuctBase::AMRObsrtuctBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	// ✨ 모든 장애물이 기본적으로 플레이어와 부딪히도록(Block) 설정합니다.
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	MeshComponent->SetNotifyRigidBodyCollision(true);
}

void AMRObsrtuctBase::BeginPlay()
{
	Super::BeginPlay();
	// ✨ OnObstacleHit 함수를 이벤트에 연결합니다.
	MeshComponent->OnComponentHit.AddDynamic(this, &AMRObsrtuctBase::OnObstacleHit);
}

void AMRObsrtuctBase::OnObstacleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	// ✨ 부딪힌 대상이 플레이어 캐릭터라면, 데미지와 넉백을 적용합니다.
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (PlayerCharacter)
	{
		UGameplayStatics::ApplyDamage(PlayerCharacter, Damage, UGameplayStatics::GetPlayerController(this, 0), this, UDamageType::StaticClass());

		FVector KnockbackDirection = -Hit.ImpactNormal;
		KnockbackDirection.Z = 0.5f;
		KnockbackDirection.Normalize();

		PlayerCharacter->LaunchCharacter(KnockbackDirection * KnockbackStrength, true, true);
	}
}