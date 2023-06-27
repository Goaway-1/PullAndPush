#include "Item/DeployableItem/BlackHoleDeployableItem.h"
#include "Components/SphereComponent.h"
#include "Interface/ItemPickupHandler.h"
#include "NiagaraFunctionLibrary.h"

ABlackHoleDeployableItem::ABlackHoleDeployableItem()
	: 
	ForceStength(10000.f),bIsBlackHoleActived(0), bIsBlackHoleEffectActived(0)
{
	EventCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("EventCollisionComp"));
	EventCollisionComp->SetupAttachment(GetRootComponent());
	EventCollisionComp->SetSphereRadius(1500.f);

	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}
void ABlackHoleDeployableItem::BeginPlay()
{
	Super::BeginPlay();

	EventCollisionComp->OnComponentBeginOverlap.AddDynamic(this,&ABlackHoleDeployableItem::AddOverlapActors);
	EventCollisionComp->OnComponentEndOverlap.AddDynamic(this, &ABlackHoleDeployableItem::DeleteOverlapActors);
}
void ABlackHoleDeployableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set Acitve Black Hole
	if (bIsBlackHoleActived) 
	{
		ActivateBlackHole();
	}
}
void ABlackHoleDeployableItem::ActiveDeployableItem()
{
	Super::ActiveDeployableItem();

	bIsBlackHoleActived = true;
	CollisionComp->SetSimulatePhysics(false);
	CollisionComp->SetEnableGravity(false);
	ProjectileMovementComponent->ProjectileGravityScale = 0;
	ProjectileMovementComponent->Velocity = FVector::Zero();
}
void ABlackHoleDeployableItem::ActivateBlackHole()
{
	// Activated Black Hole Gravity
	for (auto PrimitiveComp : OverlapComponent)
	{
		const FVector BlackHoleLocation = GetActorLocation();
		if (PrimitiveComp.IsValid()) {
			// Set Size according to distance
			FVector PrimitiveCompLocation = PrimitiveComp.Get()->GetComponentLocation();
			const float Distance = FVector::Distance(PrimitiveCompLocation, BlackHoleLocation);
			const float ForceMagnitude = ForceStength / FMath::Max(Distance, 1.f);

			// Get Direction vector
			FVector FroceVec = BlackHoleLocation - PrimitiveCompLocation;
			FroceVec.Normalize();

			// Apply Force
			PrimitiveComp.Get()->SetPhysicsLinearVelocity(FroceVec * ForceMagnitude, true);
		}
	}

	// Effect
	ensure(ExplosionEffect);
	if (!bIsBlackHoleEffectActived)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(),FRotator::ZeroRotator, FVector(0.3f));
		bIsBlackHoleEffectActived = true;
	}
}
void ABlackHoleDeployableItem::AddOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsSimulatingPhysics())
	{
		OverlapComponent.Add(OtherComp);
		
		// Exception handling if item
		TScriptInterface<class IItemPickupHandler> ActionHandler = OtherComp;
		if (ActionHandler.GetInterface()) {
			ActionHandler->SetActiveItemPickup(true);
		}
	}
}
void ABlackHoleDeployableItem::DeleteOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlapComponent.Remove(OtherComp);
}
