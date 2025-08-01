// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shakes/LegacyCameraShake.h"
#include "MRLegacyCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class MUSCLERUN_API UMRLegacyCameraShake : public ULegacyCameraShake //�𸮾󿡼� ������(���Ž�) ī�޶� ��鸲�� ���� �⺻ Ŭ����.
{
	GENERATED_BODY()

public:
	UMRLegacyCameraShake();

	// ������: ShakePattern�� �������̵��Ͽ� �� ������ ���� ����
	// virtual void UpdateShakePattern(float DeltaTime, float Alpha, FMinimalViewTarget& CurrentViewTarget) override;
public:
	// 	Ŀ���� ����ũ ������ �������Ʈ���� ������ �� �ְ� ���� ���� (���� ����).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shake")
	float MyCustomShakeAmplitude;   
};