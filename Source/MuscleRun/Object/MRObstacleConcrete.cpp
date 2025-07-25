// Fill out your copyright notice in the Description page of Project Settings.

#include "MRObstacleConcrete.h"
#include "Components/StaticMeshComponent.h" // 메시 컴포넌트 헤더 포함
#include "UObject/ConstructorHelpers.h"    // FObjectFinder를 사용하기 위한 헤더 포함

AMRObstacleConcrete::AMRObstacleConcrete()
{
    // 설계도에 따라, 구체적인 외형 설정은 여기서 합니다.
     // 1. 콘텐츠 브라우저에서 사용할 스태틱 메시 애셋의 경로를 복사합니다.
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

    // 2. 애셋 로딩에 성공했다면, 부모로부터 물려받은 MeshComponent에 설정합니다.
    if (MeshAsset.Succeeded())
    {
        // MeshComponent는 부모 클래스에 있으므로 바로 사용할 수 있습니다.
        MeshComponent->SetStaticMesh(MeshAsset.Object);
    }
}

void AMRObstacleConcrete::BeginPlay()
{
	Super::BeginPlay();
}

void AMRObstacleConcrete::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}