#include "RocketPunch/RocketPunch.h"
#include "RocketPunch/RPCollisionComponent.h"
#include "RocketPunch/RPMovementComponent.h"
#include "Components/SphereComponent.h"

ARocketPunch::ARocketPunch()
{
	RPCollisionComponent = CreateDefaultSubobject<URPCollisionComponent>(TEXT("RPCollisionComponent"));
}
void ARocketPunch::BeginPlay()
{
	Super::BeginPlay();

	// Set Overlap Collision
	CollisionComp->OnComponentHit.AddDynamic(this, &ARocketPunch::OnHit);

	// Set Weapon Out of use!
	RPMovementComponent->OnReturn.BindUObject(this, &ARocketPunch::IsOutOfUse);

	// @TEST : Set RocketPunch Force Return
	RPCollisionComponent->OnForceReturn.AddDynamic(RPMovementComponent, &URPMovementComponent::SetIsForceReturn);
	RPCollisionComponent->OnForceReturn.AddDynamic(this, &ARocketPunch::OnRPTestFunc);
}
void ARocketPunch::IsOutOfUse(const bool& Val)
{
	OutOfUse.ExecuteIfBound(Val);

	// Reset OverlapActors Array
	RPCollisionComponent->OnArrayReset.Execute();
}
void ARocketPunch::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != CasterActor) {
		RPCollisionComponent->OnHit(HitComponent,OtherActor,OtherComponent, NormalImpulse, Hit, GetCasterActor(), bIsPush, ForceAlpha);
	}
}
void ARocketPunch::SetRocketPunchCallBack(bool InValue)
{
	OnRocketPunchCallBack.ExecuteIfBound();
}