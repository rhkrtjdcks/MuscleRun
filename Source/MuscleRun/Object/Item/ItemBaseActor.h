// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBaseActor.generated.h"

/*
* 아이템의 기본 뼈대가 될 클래스입니다.
* 아이템의 효과를 제외한 모든 기본 속성들을 전부 포함합니다.
* 1. 메쉬 컴포넌트, 아이템 컴포넌트	
* 2. 오버래핑 이벤트에 함수를 바인딩하는 기능 (Activate 함수를 통한 아이템 효과 사용)
* 3-1. 오버래핑 이벤트 발생 시, UBaseEffectComponent를 외부에서 주입받아야 함.
* 3-2. ActivateItem 함수, 그리고 캐릭터 클래스에 의해 호출받고, UBasEffectComponent* 의 이펙트 적용 함수를 사용함.
*/
UCLASS(Abstract)
class MUSCLERUN_API AItemBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
