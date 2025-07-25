// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/MRPlayable.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MRPlayerCharacter.generated.h"

UCLASS()
class MUSCLERUN_API AMRPlayerCharacter : public ACharacter, public IMRPlayable
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
	UPROPERTY(VisibleAnywhere, Category = "Player")
	class UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere, Category = "Player")
	class USkeletalMeshComponent* SkeletonMesh;

	UPROPERTY(EditAnywhere, Category = "Player")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category ="Player")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Player")
	class UInputMappingContext* IMC_MRPlayerInput;

	UPROPERTY(EditAnywhere, Category = "Player")
	class UInputAction* IA_MRMove;

	UPROPERTY(EditAnywhere, Category = "Player")
	class UInputAction* IA_MTJump;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	class UMRHealthComponent* HealthComp;

private:
	void OnInputMove(const FInputActionValue& Value);
	void OnInputJump(const FInputActionValue& Value);

	void GetDamaged(float DamageAmount) override;

	void ItemActivated() override;
};
