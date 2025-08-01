// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shakes/LegacyCameraShake.h"
#include "MRLegacyCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class MUSCLERUN_API UMRLegacyCameraShake : public ULegacyCameraShake //언리얼에서 오래된(레거시) 카메라 흔들림을 위한 기본 클래스.
{
	GENERATED_BODY()

public:
	UMRLegacyCameraShake();

	// 선택적: ShakePattern을 오버라이드하여 더 복잡한 로직 구현
	// virtual void UpdateShakePattern(float DeltaTime, float Alpha, FMinimalViewTarget& CurrentViewTarget) override;
public:
	// 	커스텀 셰이크 강도를 블루프린트에서 조정할 수 있게 만든 변수 (선택 사항).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shake")
	float MyCustomShakeAmplitude;   
};