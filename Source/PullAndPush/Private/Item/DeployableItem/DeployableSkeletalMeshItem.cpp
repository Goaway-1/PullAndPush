#include "Item/DeployableItem/DeployableSkeletalMeshItem.h"

ADeployableSkeletalMeshItem::ADeployableSkeletalMeshItem()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetRootComponent());
	MeshComp->SetCollisionProfileName(MeshCollisionName);
}
void ADeployableSkeletalMeshItem::PlayAnimation()
{
	if (MeshAnimation) 
	{
		MeshComp->PlayAnimation(MeshAnimation, false);
	}
	else
	{
		PPLOG(Error, TEXT("MeshAnimation is null"));
	}
}

void ADeployableSkeletalMeshItem::DestoryDeployableItem()
{
	Super::DestoryDeployableItem();
	MeshComp->SetVisibility(false);
}
