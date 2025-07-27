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

	// 런타임에 ATileManager가 스폰할 실제 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnPoint")
	TSubclassOf<AActor> ActorClassToSpawn;
	
	// 에디터에서 디자이너가 직접 지정할 미리보기용 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnPoint|Editor")
	TObjectPtr<class UStaticMesh> PreviewMesh;

protected:
#if WITH_EDITOR
	// 에디터에서 프로퍼티가 변경될 때 호출되는 함수
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	// 미리보기 메시를 실제로 표시할 스태틱 메시 컴포넌트
	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> PreviewMeshComponent;
};