#include "Item/DeployableItem/Bomb.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

ABomb::ABomb()
	:
	ExplosionImpulse(400)
{
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(GetRootComponent());
	RadialForceComp->Radius = 250;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;

	DestoryTime = 4.f;
}
void ABomb::Explosion()
{
	ensure(ExplosionEffect);
	ensure(ExplodedMaterial);

	// Excute Explosion
	const FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
	MeshComp->AddImpulse(BoostIntensity, NAME_None, true);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	MeshComp->SetMaterial(0, ExplodedMaterial);

	RadialForceComp->FireImpulse();
}
void ABomb::ActiveDeployableItem()
{
	Super::ActiveDeployableItem();

	Explosion();
}
