#include "SpawnLocationComponent.h"
#include "Components/StaticMeshComponent.h"

USpawnLocationComponent::USpawnLocationComponent()
{
	// 미리보기 메시를 표시할 자식 컴포넌트를 생성하고 붙인다.
	PreviewMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMeshComponent"));
	PreviewMeshComponent->SetupAttachment(this);
	PreviewMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 에디터에서만 보이게 충돌은 끈다.
}

#if WITH_EDITOR
void USpawnLocationComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 변경된 프로퍼티가 'PreviewMesh' 변수인지 확인
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USpawnLocationComponent, PreviewMesh))
	{
		if (PreviewMeshComponent)
		{
			// 변수에 지정된 스태틱 메시를 자식 컴포넌트에 적용
			PreviewMeshComponent->SetStaticMesh(PreviewMesh);
		}
	}
}
#endif