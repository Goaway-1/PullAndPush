#include "Item/ActiveItem/Bomb.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

ABomb::ABomb()
	:
	ExplosionImpulse(400), ExplosionTime(3), bIsExploded(0)
{
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(GetRootComponent());
	RadialForceComp->Radius = 250;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;

	DestoryTime = 4.f;
}
void ABomb::BeginPlay()
{
	Super::BeginPlay();
	
}
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Explosion Timer
	if (!bIsExploded) {
		CheckIsExploded(DeltaTime);
	}
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
	ExplosionTime -= DeltaTime;
	if (ExplosionTime < KINDA_SMALL_NUMBER) {
		Explosion();
	}
}
