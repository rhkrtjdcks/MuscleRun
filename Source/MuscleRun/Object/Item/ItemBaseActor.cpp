// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Item/ItemBaseActor.h"

// Sets default values
AItemBaseActor::AItemBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 연결!
}

// Called when the game starts or when spawned
void AItemBaseActor::BeginPlay()
{
	Super::BeginPlay();

	// 여기서 TriggerVolume의 오버래핑 이벤트를 바인딩!
}

void AItemBaseActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 여기서 OtherActor(AMRPlayerCharacter 의 ItemActivate 함수를 통해, 호출해주세요!
	// 끝냈다면, 에디터 세팅에서 Player의 Collision Preset에서 새 Coliision Channel, Item을 만들고, Ovelap 만 활성화!
	// 이 액터의 콜리전 채널을 Item으로 하고, Player의 것도 켜 주세요!
}

