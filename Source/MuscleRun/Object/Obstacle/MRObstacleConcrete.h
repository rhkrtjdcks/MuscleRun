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

	// ✨ 부모의 OnObstacleHit 함수를 '오버라이드(재정의)'하여 기능을 확장합니다.
	virtual void OnObstacleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

private:
	FVector StartLocation;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveDistance = 500.0f;
};