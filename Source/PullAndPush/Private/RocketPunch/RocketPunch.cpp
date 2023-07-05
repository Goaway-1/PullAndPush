#include "RocketPunch/RocketPunch.h"
#include "RocketPunch/RPCollisionComponent.h"
#include "RocketPunch/RPMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ARocketPunch::ARocketPunch()
{
	CollisionComp->InitSphereRadius(20.f);
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	RPCollisionComponent = CreateDefaultSubobject<URPCollisionComponent>(TEXT("RPCollisionComponent"));
}
void ARocketPunch::BeginPlay()
{
	Super::BeginPlay();

	// Set Overlap Collision
	CollisionComp->OnComponentHit.AddDynamic(this, &ARocketPunch::OnHit);

	// Set Weapon Out of use!
	RPMovementComponent->OnReturn.BindUObject(this, &ARocketPunch::IsOutOfUse);

	// Set RocketPunch Force Return
	RPCollisionComponent->OnForceReturn.AddDynamic(RPMovementComponent, &URPMovementComponent::SetIsForceReturn);
	RPCollisionComponent->OnForceReturn.AddDynamic(this, &ARocketPunch::SetRocketPunchCallBack);
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
		RPCollisionComponent->OnHit(HitComponent,OtherActor,OtherComponent, NormalImpulse, Hit, GetCasterActor(), bIsPush, ForceAlpha, CurrentPassiveStat);
	}
}
void ARocketPunch::SetRocketPunchCallBack(bool InValue)
{
	OnRocketPunchCallBack.ExecuteIfBound();
}
void ARocketPunch::SetMeshVisibility(bool InVisibility)
{
	bStaticMeshVisibility = InVisibility;

	if (!GetOwner()->HasAuthority())
	{
		StaticMeshComp->SetVisibility(false);
	}
}
void ARocketPunch::SetMeshChange(bool IsPush)
{
	CurrentMesh = (IsPush) ? PushMesh : PullMesh;
	OnRep_ChangeMesh();		// @ERROR : 에디터에서만 오류 발생
}
void ARocketPunch::OnRep_ChangeMeshVisibility()
{
	StaticMeshComp->SetVisibility(bStaticMeshVisibility);
}
void ARocketPunch::OnRep_ChangeMesh()
{
	if (CurrentMesh)
	{
		StaticMeshComp->SetStaticMesh(CurrentMesh);
	}
}
void ARocketPunch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ARocketPunch, bStaticMeshVisibility, COND_SkipOwner);
	DOREPLIFETIME(ARocketPunch, CurrentMesh);
}