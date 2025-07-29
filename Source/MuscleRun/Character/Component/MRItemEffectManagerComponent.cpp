// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/MRItemEffectManagerComponent.h"
#include "Character/MRPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFrameWork/CharacterMovementComponent.h"

// Sets default values for this component's properties
UMRItemEffectManagerComponent::UMRItemEffectManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMRItemEffectManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

// ������ ȿ���� �����ϴ� �Լ��Դϴ�.
void UMRItemEffectManagerComponent::ApplyEffect(EItemEffectTypes ItemTypes)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	switch (ItemTypes)
	{
	case EItemEffectTypes::Score:
	{
		AMRPlayerCharacter* Player = Cast<AMRPlayerCharacter>(Owner);
		if (Player)
		{
			// ���� �⺻ �������� ȿ���� �����մϴ�. GameMode�� Score�� �÷��ִ� ������ �ۼ��غ�����!
			TempScore += 10;
			UE_LOG(LogTemp, Log, TEXT("Score +10! ���� ����: %d"), TempScore);
		}
	}
		break;
	
	case EItemEffectTypes::Faster:
	{
		ACharacter* Player = Cast<ACharacter>(Owner);
		if (Player && Player->GetCharacterMovement())
		{
			float& Speed = Player->GetCharacterMovement()->MaxWalkSpeed;
			Speed += 300.f;
			UE_LOG(LogTemp, Log, TEXT("�ӵ�����! ���� �ӵ�: %.1f"), Speed);
		}
	}
		break;
	
		// �������� �߰��� ���� ������� Enum�� �����ϰ� ���� �߰�!
	default:
		break;
	}
}
