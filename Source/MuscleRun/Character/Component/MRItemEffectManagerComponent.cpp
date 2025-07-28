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

// ������ ȿ���� �����ϴ� �Լ��Դϴ�.
void UMRItemEffectManagerComponent::ApplyEffect(EItemEffectTypes ItemTypes)
{
	switch (ItemTypes)
	{
	case EItemEffectTypes::Faster:
		return;
	case EItemEffectTypes::Score:
		// ���� �⺻ �������� ȿ���� �����մϴ�. GameMode�� Score�� �÷��ִ� ������ �ۼ��غ�����!
		return;
		// �������� �߰��� ���� ������� Enum�� �����ϰ� ���� �߰�!
	default:
		return;
	}
}
