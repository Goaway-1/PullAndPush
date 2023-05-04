#include "Item/ActiveItem/DeployableItem.h"

ADeployableItem::ADeployableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetEnableGravity(false);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComp);
}
void ADeployableItem::BeginPlay()
{
	Super::BeginPlay();
	
	SetActivePhysicsAndCollision(false);
}
void ADeployableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Destory Timer
	DestoryTime -= DeltaTime;
	if (DestoryTime < KINDA_SMALL_NUMBER)
	{
		Destroy();
	}
}
void ADeployableItem::SetActivePhysicsAndCollision(bool InActive)
{
	// Set of 'Tick, Physics, Gravity, Collision'
	SetActorTickEnabled(InActive);
	MeshComp->SetSimulatePhysics(InActive);
	MeshComp->SetEnableGravity(InActive);

	if (InActive)
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else 
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}