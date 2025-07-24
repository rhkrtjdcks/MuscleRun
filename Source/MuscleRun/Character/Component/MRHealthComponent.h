// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MRHealthComponent.generated.h"

/*
* MRPlayerCharacter�� ü���� �����ϴ� ������Ʈ�Դϴ�.
* �ʼ������� �����ؾ� �� �͵��� ������ �����ϴ�.
* 1. ü��
* 2. ü���� ȸ���ϴ� ����, ü���� ���ҵǴ� ����
* 3. ü���� �������� ��, ü���� 0�� �Ǿ��� �� ��������Ʈ�� ���� BroadCast�մϴ�.
* ���� �� Ȯ���� �ʿ伺�� �����Ŵٸ� ��ξ��� �׷��� ���ּ���! �ٸ� �̾߱����ּ���!
* UPROPERTY, UFUNCTION�� ���÷��� ���ڵ��� �����ؼ� �־��ּ���.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUSCLERUN_API UMRHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMRHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// �ַ� Character���Լ� ȣ��Ǵ� ü���� ���ҵǴ� �Լ��Դϴ�.
	void GetDamage(float Amount);

	// ü���� ȸ���ϴ� �Լ��Դϴ�.
	void Heal(float Amount);

	/*
	* ���� ��������Ʈ ���ڸ� �������ּ���!
	*/

private:
	// ü���Դϴ�.
	float Health;

	// ü���� �پ����� ���� ���¸� �˸��� ���� ��������Ʈ �����Դϴ�.
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, HealthAmount);

	// ü���� 0�� �Ǿ��� ���� ���¸� �˸��� ���� ��������Ʈ �����Դϴ�.
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthBecomeToZero);
};
