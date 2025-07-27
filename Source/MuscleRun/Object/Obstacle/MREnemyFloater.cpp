// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Obstacle/MREnemyFloater.h"

// Sets default values
AMREnemyFloater::AMREnemyFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AMREnemyFloater::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMREnemyFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 이동하고 벽에 튕기는 로직을 구현합니다.
	// 길어지거나 복잡할 경우 따로 메서드를 빼는 것을 추천합니다.
}

FORCEINLINE float AMREnemyFloater::GetEnemySpeed() const
{
	return EnemySpeed;
}

FORCEINLINE void AMREnemyFloater::SetEnemySpeed(const float NewSpeed)
{
	EnemySpeed = NewSpeed;
}

