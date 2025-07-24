// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/MRObsrtuctBase.h"

// Sets default values
AMRObsrtuctBase::AMRObsrtuctBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트를 생성하고 루트 컴포넌트에 붙입니다.
}

// Called when the game starts or when spawned
void AMRObsrtuctBase::BeginPlay()
{
	Super::BeginPlay();

	// TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AMREnemyFloater::OnOverlappingBegined);
	// 델리게이트를 여기서 바인딩합니다.
}

// Called every frame
void AMRObsrtuctBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMRObsrtuctBase::MakeDamage(AActor* OtherActor, float DamageAmount)
{
	PURE_VIRTUAL(AMRObsrtuctBase::MakeDamage,)
}

