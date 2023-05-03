#include "Item/ActiveItem/Bomb.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

ABomb::ABomb()
	:
	bIsExploded(0), ExplosionTime(3), DestoryTime(4), ExplosionImpulse(400)
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	SetRootComponent(MeshComp);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 250;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;
}
void ABomb::BeginPlay()
{
	Super::BeginPlay();
	
}
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckIsExploded(DeltaTime);
}
void ABomb::Explosion()
{
	ensure(ExplosionEffect);
	ensure(ExplodedMaterial);

	bIsExploded = true;

	// Excute Explosion
	const FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
	MeshComp->AddImpulse(BoostIntensity, NAME_None, true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	MeshComp->SetMaterial(0, ExplodedMaterial);

	RadialForceComp->FireImpulse();
}
void ABomb::CheckIsExploded(float DeltaTime)
{
	// Explosion Timer
	if (!bIsExploded)
	{
		ExplosionTime -= DeltaTime;
		if (ExplosionTime < KINDA_SMALL_NUMBER) {
			Explosion();
		}
	}
	// Destory Timer
	else
	{
		DestoryTime -= DeltaTime;
		if (DestoryTime < KINDA_SMALL_NUMBER)
		{
			Destroy();
		}
	}
}
