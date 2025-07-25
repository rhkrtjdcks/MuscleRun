// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/MRHealthComponent.h"

// Sets default values for this component's properties
UMRHealthComponent::UMRHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UMRHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	// ...
	
}


// Called every frame
void UMRHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMRHealthComponent::GetDamage(float HealthAmount)
{
	// 데미지가 0이하거나 이미 체력이 0이되면 함수 종료
	if (HealthAmount <= 0.0f || CurrentHealth <= 0.0f)
	{
		return;
	}
	CurrentHealth = FMath::Clamp(CurrentHealth - HealthAmount, 0.0f, MaxHealth);	//데미지를 깍고 0~최대체력 사이로 제한 clamp함수 호출
	OnHealthChanged.Broadcast(CurrentHealth);	// 체력이 바뀌었으니, 바인딩 된 대상 UI등에 새 체력값 알려줌
	if (CurrentHealth<=0.0f)
	{
		OnHealthBecomeToZero.Broadcast();	//체력이 0이되면 죽었으니까 OnHealthZero호출
	}
}

void UMRHealthComponent::Heal(float HealthAmount)
{
	//회복량이 0이거나 이미 죽은 상태면 회복 안함
	if (HealthAmount <= 0.0f || CurrentHealth <= 0.0f)
		return;		

	CurrentHealth = FMath::Clamp(CurrentHealth + HealthAmount, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);				//체력을 회복하고, Clampo로 제한 후 이벤트 발생
}

