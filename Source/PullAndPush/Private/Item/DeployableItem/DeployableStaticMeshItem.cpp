#include "Item/DeployableItem/DeployableStaticMeshItem.h"

ADeployableStaticMeshItem::ADeployableStaticMeshItem()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetRootComponent());
}
