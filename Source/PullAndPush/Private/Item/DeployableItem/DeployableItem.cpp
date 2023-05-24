#include "Item/DeployableItem/DeployableItem.h"
#include "Net/UnrealNetwork.h"

ADeployableItem::ADeployableItem()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

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

	// Destory Timer
	SetActivePhysicsAndCollision(false);
}
void ADeployableItem::ActiveDeployableItem()
{
	PPLOG(Log, TEXT("Set Active DeployableItem"));
}
void ADeployableItem::DestoryDeployableItem()
{
	PPLOG(Log, TEXT("Destroy DeployableItem"));
	Destroy();
}
void ADeployableItem::SetActivePhysicsAndCollision(bool InActive)
{
	// Set of 'Tick, Physics, Gravity, Collision'
	bActivePhysicsAndCollision = InActive;
	SetActorTickEnabled(InActive);
	MeshComp->SetSimulatePhysics(InActive);
	MeshComp->SetEnableGravity(InActive);

	if (InActive)
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetWorld()->GetTimerManager().SetTimer(ActiveHandler, this, &ADeployableItem::ActiveDeployableItem, ActiveTime, false);
		GetWorld()->GetTimerManager().SetTimer(DestoryHandler, this, &ADeployableItem::DestoryDeployableItem, DestoryTime, false);
	}
	else 
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
void ADeployableItem::OnRep_ChangePhysicsAndCollision()
{
	MeshComp->SetSimulatePhysics(bActivePhysicsAndCollision);
	MeshComp->SetEnableGravity(bActivePhysicsAndCollision);

	if (bActivePhysicsAndCollision)
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
void ADeployableItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeployableItem, bActivePhysicsAndCollision);
}