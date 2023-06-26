#include "Item/DeployableItem/DeployableSkeletalMeshItem.h"

ADeployableSkeletalMeshItem::ADeployableSkeletalMeshItem()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetRootComponent());
}
