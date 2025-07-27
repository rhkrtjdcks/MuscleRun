#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpawnLocationComponent.generated.h"

UENUM()
enum class ESpawnActorType { Obstacle, Item };

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MUSCLERUN_API USpawnLocationComponent : public USceneComponent
{
    GENERATED_BODY()

public:	
	USpawnLocationComponent();

	// ��Ÿ�ӿ� ATileManager�� ������ ���� ���� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnPoint")
	TSubclassOf<AActor> ActorClassToSpawn;
	
	// �����Ϳ��� �����̳ʰ� ���� ������ �̸������ �޽�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnPoint|Editor")
	TObjectPtr<class UStaticMesh> PreviewMesh;

protected:
#if WITH_EDITOR
	// �����Ϳ��� ������Ƽ�� ����� �� ȣ��Ǵ� �Լ�
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	// �̸����� �޽ø� ������ ǥ���� ����ƽ �޽� ������Ʈ
	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> PreviewMeshComponent;
};