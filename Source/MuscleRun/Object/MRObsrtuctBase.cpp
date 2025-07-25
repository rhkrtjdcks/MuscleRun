// Fill out your copyright notice in the Description page of Project Settings.


#include "MRObsrtuctBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Character/MRPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"



// Sets default values
AMRObsrtuctBase::AMRObsrtuctBase()
{
	// 컴포넌트를 생성하고 루트 컴포넌트에 붙입니다.
    //MeshComponent = CreateDefaultSubobject<UMeshComponent>("MeshComp");
    // ...
     // 이 클래스 자체는 틱을 사용하지 않으므로 꺼두는 것이 효율적입니다.
    PrimaryActorTick.bCanEverTick = false;

    // 1. 충돌을 감지할 박스(TriggerVolume)를 생성하고 액터의 루트(중심)로 설정합니다.
    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    TriggerVolume->SetupAttachment(RootComponent);

    // 2. 외형을 보여줄 메시(MeshComponent)를 생성합니다.
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

    // 3. 메시를 루트(TriggerVolume)에 붙여서 함께 움직이도록 합니다.
    MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMRObsrtuctBase::BeginPlay()
{
	Super::BeginPlay();

    // 델리게이트를 여기서 바인딩합니다.
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AMRObsrtuctBase::OnOverlapBegin);

}

void AMRObsrtuctBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AMRPlayerCharacter* PlayerCharacter = Cast<AMRPlayerCharacter>(OtherActor);
    if (!PlayerCharacter)
    {
        return;
    }
    ApplyObstacleDamage(PlayerCharacter);

    // 작동 확인용 로그 메시지
    UE_LOG(LogTemp, Warning, TEXT("플레이어와 충돌! 데미지를 입혔습니다."));
}

void AMRObsrtuctBase::ApplyObstacleDamage(AActor* OtherActor)
{
    // 실무자(MakeDamage)를 호출합니다.
      // 데미지 값은 이 클래스가 원래 가지고 있던 'Damage' 변수를 넘겨줍니다.
    // OtherActor->MakeDamage(Damage);
}

