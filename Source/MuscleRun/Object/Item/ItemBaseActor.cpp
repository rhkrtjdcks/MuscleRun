// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Item/ItemBaseActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/MRPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItemBaseActor::AItemBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(RootComponent);
	// TriggerVolume->SetBoxExtent()

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetupAttachment(RootComponent);
	// ������Ʈ ����!
}

// Called when the game starts or when spawned
void AItemBaseActor::BeginPlay()
{
	Super::BeginPlay();

	// ���⼭ TriggerVolume�� �������� �̺�Ʈ�� ���ε�!
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AItemBaseActor::OnOverlapBegin);
}

void AItemBaseActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ���⼭ OtherActor(AMRPlayerCharacter �� ItemActivate �Լ��� ����, ȣ�����ּ���!
	// ���´ٸ�, ������ ���ÿ��� Player�� Collision Preset���� �� Coliision Channel, Item�� �����, Ovelap �� Ȱ��ȭ!
	// �� ������ �ݸ��� ä���� Item���� �ϰ�, Player�� �͵� �� �ּ���!
	AMRPlayerCharacter* PlayerCharacter = Cast<AMRPlayerCharacter>(OtherActor);
	if (PlayerCharacter && ItemType != EItemEffectTypes::None)
	{
		PlayerCharacter->ItemActivated(ItemType);
		Destroy();
	}
}

