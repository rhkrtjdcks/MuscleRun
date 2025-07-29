// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Item/ItemBaseActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/MRPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AItemBaseActor::AItemBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComp);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(RootComponent);
	// TriggerVolume->SetBoxExtent()

	// 컴포넌트 연결!
}

// Called when the game starts or when spawned
void AItemBaseActor::BeginPlay()
{
	Super::BeginPlay();

	// 여기서 TriggerVolume의 오버래핑 이벤트를 바인딩!
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AItemBaseActor::OnOverlapBegin);
}

void AItemBaseActor::Destroyed()
{
	Super::Destroyed();

	if (VanishSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, VanishSound, GetActorLocation());
	}

	if (VanishEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, VanishEffect, GetActorLocation());
	}

	Destroy();
}

void AItemBaseActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 여기서 OtherActor(AMRPlayerCharacter 의 ItemActivate 함수를 통해, 호출해주세요!
	// 끝냈다면, 에디터 세팅에서 Player의 Collision Preset에서 새 Coliision Channel, Item을 만들고, Ovelap 만 활성화!
	// 이 액터의 콜리전 채널을 Item으로 하고, Player의 것도 켜 주세요!
	AMRPlayerCharacter* PlayerCharacter = Cast<AMRPlayerCharacter>(OtherActor);
	if (PlayerCharacter && ItemType != EItemEffectTypes::None)
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSoundCue, GetActorLocation());
		}

		if (PickupEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupEffect, GetActorLocation());
		}
		/*UE_LOG(LogTemp, Warning, TEXT("충돌: %s(%s) 와 %s(%s)"),
			*GetName(), *OverlappedComp->GetName(),
			*OtherActor->GetName(), *OtherComp->GetName());*/
		PlayerCharacter->ItemActivated(ItemType);

		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AItemBaseActor::DestroySelf, 0.8f, false);
	}
}

void AItemBaseActor::DestroySelf()
{
	Destroy();
}

