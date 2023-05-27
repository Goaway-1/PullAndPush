#include "Item/DeployableItem/DeployableItem.h"

ADeployableItem::ADeployableItem()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetEnableGravity(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SetRootComponent(MeshComp);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(MeshComp);
	ProjectileMovementComponent->InitialSpeed = 1300.f;
}
void ADeployableItem::BeginPlay()
{
	Super::BeginPlay();

	if(ActiveTime < KINDA_SMALL_NUMBER) PPLOG(Warning,TEXT("ActiveTime is small than 0"));
	if(DestoryTime < KINDA_SMALL_NUMBER) PPLOG(Warning,TEXT("ActiveTime is small than 0"));

	// Set Timer
	GetWorld()->GetTimerManager().SetTimer(ActiveHandler, this, &ADeployableItem::ActiveDeployableItem, ActiveTime, false);
	GetWorld()->GetTimerManager().SetTimer(DestoryHandler, this, &ADeployableItem::DestoryDeployableItem, DestoryTime, false);
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