// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Obstructable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObstructable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 장애물 액터에게 자격을 부여하는 인터페이스입니다.
 * 모든 장애물 액터는 AActor와 이 클래스를 상속해야만 합니다.
 */
class MUSCLERUN_API IObstructable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	/*
	* 인터페이스에서 자격을 부여할 가상함수입니다.
	* 반드시 하위 클래스에서 자격을 부여하고 상속해주세요.
	*/ 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Obstruct")
	void MakeDamage(float DamageAmount);
};
