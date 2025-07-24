// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MRPlayerCharacter.generated.h"

UCLASS()
class MUSCLERUN_API AMRPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMRPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "BoxComp")
	class UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere, Category = "SkeleMeshComp")
	class USkeletalMeshComponent* SkeletonMesh;

	UPROPERTY(EditAnywhere, Category = "SpringcComp")
	class USpringArmComponent* SpringArm;
};
