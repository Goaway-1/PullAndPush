#include "Item/DeployableItem/Bomb.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/CharacterStatHandler.h"
#include "PhysicsEngine/RadialForceComponent.h"

ABomb::ABomb()
{
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(GetRootComponent());
	RadialForceComp->Radius = ExplosionRadius;
	RadialForceComp->ImpulseStrength = ExplosionImpulse;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;

	ModifyCharacterStat = ECharacterStat::Stun;
	DestoryTime = 4.f;
}
void ABomb::Explosion()
{
	ensure(ExplosionEffect);
	ensure(ExplodedMaterial);

	// Excute Explosion My Self
	const FVector BoostIntensity = FVector::UpVector * SelfExplosionImpulse;
	MeshComp->AddImpulse(BoostIntensity, NAME_None, true);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	MeshComp->SetMaterial(0, ExplodedMaterial);

	// Excute Explosion for Another Actors
	RadialForceComp->FireImpulse();
	ModifyOverlappingCharacterStates();
}
void ABomb::ModifyOverlappingCharacterStates()
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
					CharacterHandler->EnableStatFlag(ModifyCharacterStat, StatChangeDuration);
				}
				UniqueActors.Add(HitActor);
			}
		}
	}
}
void ABomb::ActiveDeployableItem()
{
	Super::ActiveDeployableItem();

	Explosion();
}