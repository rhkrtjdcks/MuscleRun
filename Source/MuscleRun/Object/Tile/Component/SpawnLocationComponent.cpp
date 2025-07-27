#include "SpawnLocationComponent.h"
#include "Components/StaticMeshComponent.h"

USpawnLocationComponent::USpawnLocationComponent()
{
	// �̸����� �޽ø� ǥ���� �ڽ� ������Ʈ�� �����ϰ� ���δ�.
	PreviewMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMeshComponent"));
	PreviewMeshComponent->SetupAttachment(this);
	PreviewMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �����Ϳ����� ���̰� �浹�� ����.
}

#if WITH_EDITOR
void USpawnLocationComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// ����� ������Ƽ�� 'PreviewMesh' �������� Ȯ��
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USpawnLocationComponent, PreviewMesh))
	{
		if (PreviewMeshComponent)
		{
			// ������ ������ ����ƽ �޽ø� �ڽ� ������Ʈ�� ����
			PreviewMeshComponent->SetStaticMesh(PreviewMesh);
		}
	}
}
#endif