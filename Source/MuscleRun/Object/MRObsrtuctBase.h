// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MRObsrtuctBase.generated.h"

UCLASS(Abstract)
class MUSCLERUN_API AMRObsrtuctBase : public AActor
{
	GENERATED_BODY()

public:
	AMRObsrtuctBase();

protected:
	// 메쉬 컴포넌트입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstruct", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	// 데미지 양을 저장할 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstruct", meta = (AllowPrivateAccess = "true"))
	float Damage = 25.0f;

	// ✨ 플레이어를 튕겨낼 힘의 크기를 저장할 변수를 추가합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstruct", meta = (AllowPrivateAccess = "true"))
	float KnockbackStrength = 700.0f;

private:
	// 컴포넌트들의 기준점이 될 루트 컴포넌트입니다.
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneRoot;
};
