#include "Item/DeployableItem/DeployableStaticMeshItem.h"

ADeployableStaticMeshItem::ADeployableStaticMeshItem()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetRootComponent());
	MeshComp->SetCollisionProfileName(MeshCollisionName);
}

void ADeployableStaticMeshItem::DestoryDeployableItem()
{
	Super::DestoryDeployableItem();
	MeshComp->SetVisibility(false);
}
