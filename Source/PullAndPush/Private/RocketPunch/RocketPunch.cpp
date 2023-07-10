#include "RocketPunch/RocketPunch.h"
#include "RocketPunch/RPMovementComponent.h"
#include "RocketPunch/RPCollisionComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"

ARocketPunch::ARocketPunch()
	:
	bIsServerRP(0), bIsPush(false)
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(20.f);
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetEnableGravity(false);
	CollisionComp->SetNotifyRigidBodyCollision(true);
	CollisionComp->SetGenerateOverlapEvents(false);
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	SetRootComponent(CollisionComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetWorldScale3D(FVector(0.3f));
	StaticMeshComp->SetCollisionProfileName(NoCollisionName);
	StaticMeshComp->SetupAttachment(GetRootComponent());
	StaticMeshComp->SetIsReplicated(true);

	RPMovementComponent = CreateDefaultSubobject<URPMovementComponent>(TEXT("RPMovementComponent"));
	RPCollisionComponent = CreateDefaultSubobject<URPCollisionComponent>(TEXT("RPCollisionComponent"));
}
void ARocketPunch::BeginPlay()
{
	Super::BeginPlay();

	ensure(PushMesh);
	ensure(PullMesh);

	// Set Overlap Collision
	CollisionComp->OnComponentHit.AddDynamic(this, &ARocketPunch::OnHit);

	// Set Weapon Out of use!
	RPMovementComponent->OnReturn.BindUObject(this, &ARocketPunch::IsOutOfUse);

	// Set RocketPunch Force Return
	RPCollisionComponent->OnForceReturn.BindUObject(RPMovementComponent, &URPMovementComponent::SetIsForceReturn);

	SetMeshVisibility(false);
}
void ARocketPunch::ReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, FItemEnhancedStat InPassiveStat)
{
	if (!HasAuthority() && bIsServerRP)
	{
		ServerReadyToLaunch(InForceAlpha, InCasterActor, IsPush, InVec, InRot, InPassiveStat);
	}
	else
	{
		if (!CasterActor.IsValid()) CasterActor = InCasterActor;

		// Set Scale of Visual & Collision
		CurrentPassiveStat = InPassiveStat;
		CurrentScale = (InPassiveStat.RPScale < KINDA_SMALL_NUMBER) ? FVector(1.f) : FVector(InPassiveStat.RPScale);
		OnRep_ChangeScale();

		bIsPush = IsPush;
		ForceAlpha = InForceAlpha;
		RPMovementComponent->Launch(ForceAlpha, CasterActor.Get(), InVec, InRot, InPassiveStat);
		PPLOG(Log, TEXT("AlphaSpeed : %f, AlphaRange : %f, AlphaSize : %s"), InPassiveStat.RPSpeed, InPassiveStat.RPRange, *CurrentScale.ToString());

		// Setting Static mesh of RP
		SetMeshChange(bIsPush);
	}
}
void ARocketPunch::ServerReadyToLaunch_Implementation(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, FItemEnhancedStat InPassiveStat)
{
	ReadyToLaunch(InForceAlpha, InCasterActor, IsPush, InVec, InRot, InPassiveStat);
}
void ARocketPunch::IsOutOfUse(const bool& Val)
{
	OutOfUse.ExecuteIfBound(Val);

	// Reset OverlapActors Array
	RPCollisionComponent->OnArrayReset.Execute();
}
void ARocketPunch::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != CasterActor) 
	{
		RPCollisionComponent->OnHit(HitComponent, OtherActor, OtherComponent, NormalImpulse, Hit, GetCasterActor(), bIsPush, bIsServerRP, ForceAlpha, CurrentPassiveStat);
	}
}
void ARocketPunch::SetForceReturn()
{
	PPLOG(Warning, TEXT("SetForceReturn"));
	RPMovementComponent->SetIsForceReturn(true);
}
void ARocketPunch::SetCollisionSimulatePhysics(bool Val)
{
	if (CollisionComp) CollisionComp->SetSimulatePhysics(Val);
}
void ARocketPunch::SetMeshChange(bool IsPush)
{
	CurrentMesh = (IsPush) ? PushMesh : PullMesh;
	OnRep_ChangeMesh();
}
void ARocketPunch::SetMeshVisibility(bool InVisibility)
{
	if (!bIsServerRP)
	{
		bStaticMeshVisibility = InVisibility;
	}
	else
	{
		bStaticMeshVisibility = false;
	}
	OnRep_ChangeMeshVisibility();
}
void ARocketPunch::OnRep_ChangeScale()
{
	CollisionComp->SetWorldScale3D(CurrentScale);
	StaticMeshComp->SetWorldScale3D(CurrentScale);
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