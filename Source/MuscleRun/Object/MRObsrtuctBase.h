// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MRObsrtuctBase.generated.h"

/*
* 장애물이 데미지를 입히거나, 오버래핑을 일으키는 추상적 로직이 일어나는 클래스입니다.
* 메쉬나 더욱 구체화된 조절은 MRObstacleConcrete에서 하기로 합니다.
*/
UCLASS(Abstract)
class MUSCLERUN_API AMRObsrtuctBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMRObsrtuctBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 메쉬 컴포넌트입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstruct", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	// 오버래핑 이벤트 발생을 위한 트리거 볼륨입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstruct", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerVolume;

	UFUNCTION(BlueprintCallable, Category = "Obstruct")
	virtual void MakeDamage(AActor* OtherActor, float DamageAmount);

	// 오버래핑 시작 시 함수입니다. 요구 인자가 매우 많으니 검색해서 작성해주세요.
	// UFUNCTION() // 델리게이트에 바인딩할 함수는 UFUNCTION 리플렉션 매크로가 필요합니다.
	// void OnOvelappingStarted(...);

};
