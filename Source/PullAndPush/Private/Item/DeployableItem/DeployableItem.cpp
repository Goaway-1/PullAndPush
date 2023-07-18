#include "Item/DeployableItem/DeployableItem.h"

ADeployableItem::ADeployableItem()
	:
	bIsAutoActive(1)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSimulatePhysics(false);
	CollisionComp->SetEnableGravity(true);
	CollisionComp->SetCollisionProfileName(CollisionName);
	SetRootComponent(CollisionComp);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComp);
	ProjectileMovementComponent->InitialSpeed = 1300.f;
	ProjectileMovementComponent->bShouldBounce = true;
}
void ADeployableItem::BeginPlay()
{
	Super::BeginPlay();

	// Activate timer Automatically or Not
	if (bIsAutoActive)
	{
		SetActiveTimer();
	}
	else 
	{
		ActiveTime = 0.1f;
		SetActorRotation(FRotator::ZeroRotator);
		ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &ADeployableItem::OnProjectileBounce);
	}
}
void ADeployableItem::ActiveDeployableItem()
{
	PPLOG(Log, TEXT("Set Active DeployableItem"));
}
void ADeployableItem::DestoryDeployableItem()
{
	PPLOG(Log, TEXT("Destroy DeployableItem"));

	GetWorld()->GetTimerManager().ClearTimer(ActiveHandler);
	GetWorld()->GetTimerManager().ClearTimer(DestoryHandler);

	SetLifeSpan(1.5f);
}
void ADeployableItem::SetActiveTimer()
{
	if (ActiveTime < KINDA_SMALL_NUMBER) PPLOG(Log, TEXT("ActiveTime is small than 0"));
	if (DestoryTime < KINDA_SMALL_NUMBER) PPLOG(Log, TEXT("ActiveTime is small than 0"));

	// Set Timer
	GetWorld()->GetTimerManager().SetTimer(ActiveHandler, this, &ADeployableItem::ActiveDeployableItem, ActiveTime, false);
	GetWorld()->GetTimerManager().SetTimer(DestoryHandler, this, &ADeployableItem::DestoryDeployableItem, DestoryTime, false);
}
void ADeployableItem::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (!bIsAutoActive)
	{
		SetActiveTimer();
		bIsAutoActive = true;
	}
}
