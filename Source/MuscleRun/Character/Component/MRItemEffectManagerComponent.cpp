// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/MRItemEffectManagerComponent.h"
#include "Character/MRPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>

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
	switch (ItemTypes)
	{
		case EItemEffectTypes::Score:
		{
			// 여기 기본 아이템의 효과를 적용합니다. GameMode의 Score를 올려주는 로직을 작성해보세요!
			TempScore += 10;
			UE_LOG(LogTemp, Log, TEXT("Score +10! 현재 점수: %d"), TempScore);

			TObjectPtr<AMRGameState> CachedGameState = Cast<AMRGameState>(UGameplayStatics::GetGameState(this));
			CachedGameState->AddScore(10);
			break;
		}
	
		case EItemEffectTypes::Faster:
		{
			ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
			if (PlayerCharacter)
			{
			UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement();
			MoveComp->MaxWalkSpeed += 300.f;
			UE_LOG(LogTemp, Log, TEXT("속도증가! 현재 속도: %.1f"), MoveComp->MaxWalkSpeed);
			}
			break;
		}
	
		/*case  EItemEffectTypes::NoDie:
		{
			ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
			if (PlayerCharacter)
			{
				AMRPlayerCharacter* MRCharacter = Cast<AMRPlayerCharacter>(PlayerCharacter);
				if (MRCharacter)
				{
					MRCharacter->bIsInvincible = true;

					//(선택) 일정 시간 후 다시 죽을 수 있도록 타이머 설정
					FTimerHandle TimerHandle;
					MRCharacter->GetWorldTimerManager().SetTimer(
					TimerHandle, [MRCharacter]() 
					{MRCharacter->bIsInvincible = false; }, 
					5.0f, // 불사 지속 시간 (초)
					false);
				}
			}
			break;
		}*/
		default:
		break;
	}	
}

/*무적은 캐릭터에  MRPlayerCharacter.h
public:
	UPROPERTY(BlueprintReadWrite, Category = "Effect")
	bool bIsInvincible = false;
	
	소스에
	void AMRPlayerCharacter::TakeDamage(float Damage)
	{
		if (bIsInvincible)
		{
			// 무적 상태이므로 피해 무시
			return;
		}

		// 일반적인 피해 처리...
	}*/