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

// 각각의 효과를 구현하는 함수입니다.
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
			// 여기 기본 아이템의 효과를 적용합니다. GameMode의 Score를 올려주는 로직을 작성해보세요!
			UE_LOG(LogTemp, Warning, TEXT("충돌된 아이템 타입: %d"), static_cast<int32>(ItemTypes));
			TempScore += 10;
			UE_LOG(LogTemp, Log, TEXT("Score +10! 현재 점수: %d"), TempScore);
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
			UE_LOG(LogTemp, Log, TEXT("속도증가! 현재 속도: %.1f"), Speed);
		}
	}
		break;
	
		// 아이템을 추가할 때는 헤더에서 Enum을 수정하고 여기 추가!
	default:
		break;
	}
}
