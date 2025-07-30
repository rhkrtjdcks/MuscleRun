// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MRItemEffectManagerComponent.generated.h"

// 아이템 타입입니다. 여기 정의한 타입들을 받아 아이템 효과를 switch 안에서 구현합니다.
UENUM(BlueprintType)
enum class EItemEffectTypes : uint8
{
	None,
	Score,
	Faster,
	NoDie
};

/*
* 효과를 적용하는 컴포넌트입니다.
* 이 컴포넌트는 Player의 속성을 변경시킵니다.
* ApplyEffect의 인자에 따라 GetOwner로 Player의 속성을 변화시킵니다.
* 클래스 구성을 간략화해서, 더 이상 캐릭터는 EndDelegate를 사용하지 않습니다. 무적 시간 같은 로직을 사용 시, 타이머를 이용해주세요.
*/
UCLASS(Blueprintable)
class MUSCLERUN_API UMRItemEffectManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMRItemEffectManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// 여기서 모든 효과를 처리합니다!
	void ApplyEffect(EItemEffectTypes ItemTypes);
		
private:
	int32 TempScore = 0;
};
