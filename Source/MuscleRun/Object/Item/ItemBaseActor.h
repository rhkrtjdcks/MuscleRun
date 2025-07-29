// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/MRItemEffectManagerComponent.h"
#include "GameFramework/Actor.h"
#include "ItemBaseActor.generated.h"

/*
* �������� �⺻ ���밡 �� Ŭ�����Դϴ�.
* �������� ȿ���� ������ ��� �⺻ �Ӽ����� ���� �����մϴ�.
* ������ ����ȭ�ؼ� ������ ���� ��ɸ��� �����մϴ�!
* 1. ������Ʈ��
* 2. �������� ���ε� �Լ�.
*/
UCLASS()
class MUSCLERUN_API AItemBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	class UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere, Category = "Item")
	class UStaticMeshComponent* MeshComp;

	// ������ Ÿ���� �����Ϳ� �־��ּ���.
	UPROPERTY(EditAnywhere, Category = "Item")
	EItemEffectTypes ItemType;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* PickupSoundCue;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* VanishSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* PickupEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* VanishEffect;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DestroySelf();
};
