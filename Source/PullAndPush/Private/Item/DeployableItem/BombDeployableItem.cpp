#include "Item/DeployableItem/BombDeployableItem.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/CharacterStatHandler.h"
#include "PhysicsEngine/RadialForceComponent.h"

ABombDeployableItem::ABombDeployableItem()
{
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(GetRootComponent());
	RadialForceComp->Radius = ExplosionRadius;
	RadialForceComp->ImpulseStrength = ExplosionImpulse;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;

	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	CharacterStatModifier.Stat = ECharacterStat::Stun;
	CharacterStatModifier.ChangeDuration = 4.f;
}
void ABombDeployableItem::Explosion()
{
	ensure(ExplosionEffect);
	ensure(ExplodedMaterial);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	MeshComp->SetMaterial(0, ExplodedMaterial);

	// Excute Explosion for Another Actors
	RadialForceComp->FireImpulse();
	ModifyOverlappingCharacterStates();
}
void ABombDeployableItem::ModifyOverlappingCharacterStates()
{
	TArray<FHitResult> OutHits;
	FVector CurrentLocation = GetActorLocation();
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(ExplosionRadius);

	// Draw Collision Sphere
	DrawDebugSphere(GetWorld(), GetActorLocation(), CollisionSphere.GetSphereRadius(), 50, FColor::Cyan, false, 3.f);
	bool bIsHit = GetWorld()->SweepMultiByChannel(OutHits, CurrentLocation, CurrentLocation, FQuat::Identity, ECC_WorldStatic, CollisionSphere);

	if (bIsHit)
	{
		TSet<AActor*> UniqueActors;				// Deduplication prevention
		for (auto& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && !UniqueActors.Contains(HitActor))
			{
				TScriptInterface<ICharacterStatHandler> CharacterHandler = HitActor;
				if (CharacterHandler.GetInterface())
				{
					CharacterHandler->EnableStatFlag(CharacterStatModifier.Stat, CharacterStatModifier.ChangeDuration);
				}
				UniqueActors.Add(HitActor);
			}
		}
	}
}
void ABombDeployableItem::ActiveDeployableItem()
{
	Super::ActiveDeployableItem();

	Explosion();
}