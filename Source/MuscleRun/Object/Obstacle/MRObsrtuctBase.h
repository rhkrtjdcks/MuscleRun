// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/Boxcomponent.h"
#include "MRObsrtuctBase.generated.h"

/*
* 장애물이 데미지를 입히거나, 오버래핑을 일으키는 추상적 로직이 일어나는 클래스입니다.
* 메쉬나 더욱 구체화된 조절은 MRObstacleConcrete에서 하기로 합니다.
*/
// <<추상 클래스>>
// 모든 장애물의 공통적인 동작, 오퍼레이션을 정의하는 추상 부모 클래스입니다.
// 이 클래스를 직접적으로 생성할 수 없고, 반드시 상속받아 사용해야 합니다.

UCLASS(Abstract) // Abstract 키워드로 추상 클래스임을 명시합니다.
class MUSCLERUN_API AMRObsrtuctBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMRObsrtuctBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Getter
	FORCEINLINE float GetDamage() const { return Damage; }

	// Setter
	FORCEINLINE void GetDamage(const float NewDamage) { Damage = NewDamage; }

protected:
	// 메쉬 컴포넌트입니다.
	// 외부에서 직접 접근할 수 없고록 protected로 선언
	// 3D 모델을 나타내는 Static Mesh Component
	// meta = (AllowPrivateAccess = "true")는 "이 private 변수에도 언리얼 엔진의 
	// 블루프린트에서 접근할 수 있도록 허용해줘!" 라는 의미입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstruct", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	// 오버래핑 이벤트 발생을 위한 트리거 볼륨(특정 영역에 들어오거나 나가는 이벤트를 감지하고 반응하도록 설정된 게임 오브젝트)입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstruct", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerVolume;
	
	// 데미지 양을 저장할 변수입니다. EditAnywhere로 에디터에서 값을 수정할 수 있게합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstruct", meta = (AllowPrivateAccess = "true"))
	float Damage = 25.0f;

private:
	// 델리게이트에 바인딩할 함수는 UFUNCTION() 매크로가 꼭 필요합니다.
	// 플레이어가 TriggerVolume에 들어왔을 때 호출될 함수입니다.
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// 데미지를 입히는 함수입니다. 이제 가상이 아니어도 괜찮습니다.
	//BlueprintCallable을 통해 블루프린트에서도 이 함수를 호출할 수 있습니다.
	UFUNCTION(BlueprintCallable, Category = "Obstruct")
	void ApplyObstacleDamage(AActor* OtherActor);
};
