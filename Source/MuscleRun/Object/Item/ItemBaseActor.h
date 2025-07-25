// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBaseActor.generated.h"

/*
* �������� �⺻ ���밡 �� Ŭ�����Դϴ�.
* �������� ȿ���� ������ ��� �⺻ �Ӽ����� ���� �����մϴ�.
* 1. �޽� ������Ʈ, ������ ������Ʈ	
* 2. �������� �̺�Ʈ�� �Լ��� ���ε��ϴ� ��� (Activate �Լ��� ���� ������ ȿ�� ���)
* 3-1. �������� �̺�Ʈ �߻� ��, UBaseEffectComponent�� �ܺο��� ���Թ޾ƾ� ��.
* 3-2. ActivateItem �Լ�, �׸��� ĳ���� Ŭ������ ���� ȣ��ް�, UBasEffectComponent* �� ����Ʈ ���� �Լ��� �����.
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
