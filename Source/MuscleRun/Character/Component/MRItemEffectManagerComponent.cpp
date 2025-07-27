// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/MRItemEffectManagerComponent.h"

// Sets default values for this component's properties
UMRItemEffectManagerComponent::UMRItemEffectManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMRItemEffectManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// 각각의 효과를 구현하는 함수입니다.
void UMRItemEffectManagerComponent::ApplyEffect(EItemEffectTypes ItemTypes)
{
	switch (ItemTypes)
	{
	case EItemEffectTypes::Faster:
		return;
	case EItemEffectTypes::Score:
		// 여기 기본 아이템의 효과를 적용합니다. GameMode의 Score를 올려주는 로직을 작성해보세요!
		return;
		// 아이템을 추가할 때는 헤더에서 Enum을 수정하고 여기 추가!
	default:
		return;
	}
}
