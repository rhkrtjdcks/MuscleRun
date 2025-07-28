// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MRObsrtuctBase.h"
#include "MRObstacleConcrete.generated.h"

class UProjectileMovementComponent;

UCLASS()
class MUSCLERUN_API AMRObstacleConcrete : public AMRObsrtuctBase
{
	GENERATED_BODY()

public:
	AMRObstacleConcrete();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 메쉬가 다른 컴포넌트와 부딪혔을 때 호출될 함수
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// 발사체 움직임을 담당하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

private:
	// 초기 위치 저장 변수
	FVector StartLocation;

	// 이동할 거리
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveDistance = 500.0f;
};