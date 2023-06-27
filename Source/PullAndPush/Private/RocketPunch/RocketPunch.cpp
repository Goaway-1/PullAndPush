#include "RocketPunch/RocketPunch.h"
#include "RocketPunch/RPMovementComponent.h"
#include "RocketPunch/RPCollisionComponent.h"
#include "Interface/CharacterStatHandler.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

ARocketPunch::ARocketPunch()
	: 
	bIsPush(false)
{
 	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(35.0f);
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetEnableGravity(false);
	CollisionComp->SetNotifyRigidBodyCollision(true);
	CollisionComp->SetGenerateOverlapEvents(false);
	CollisionComp->SetCollisionProfileName(CollisionName);
	SetRootComponent(CollisionComp);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
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
}
void ARocketPunch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ARocketPunch::ReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, const float& DeltaSpeed, const float& DeltaRange, const float& DeltaScale)
{
	if (!HasAuthority()) 
	{
		ServerReadyToLaunch(InForceAlpha, InCasterActor, IsPush, InVec, InRot, DeltaSpeed, DeltaRange, DeltaScale);
	}
	else 
	{
		if (!CasterActor.IsValid()) CasterActor = InCasterActor;

		// Set Scale of Visual & Collision
		CurrentScale = FVector(DeltaScale);
		CollisionComp->SetWorldScale3D(CurrentScale);
		StaticMeshComp->SetWorldScale3D(CurrentScale);

		bIsPush = IsPush;
		ForceAlpha = InForceAlpha;
		RPMovementComponent->Launch(ForceAlpha, CasterActor.Get(), InVec, InRot, DeltaSpeed, DeltaRange);
		PPLOG(Log, TEXT("AlphaSpeed : %f, AlphaRange : %f, AlphaSize : %s"), DeltaSpeed, DeltaRange, *CurrentScale.ToString());

		// Setting Static mesh of RP
		CurrentMesh = (bIsPush) ? PushMesh : PullMesh;
		OnRep_ChangeMesh();
	}
}
void ARocketPunch::ServerReadyToLaunch_Implementation(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, const float& DeltaSpeed, const float& DeltaRange, const float& DeltaScale)
{
	ReadyToLaunch(InForceAlpha, InCasterActor, IsPush, InVec, InRot, DeltaSpeed, DeltaRange, DeltaScale);
}
void ARocketPunch::IsOutOfUse(const bool& Val)
{
	OutOfUse.Execute(Val);

	// Reset OverlapActors Array
	RPCollisionComponent->OnArrayReset.Execute();
}
void ARocketPunch::SetCollisionSimulatePhysics(bool Val)
{
	if(CollisionComp) CollisionComp->SetSimulatePhysics(Val);
}
void ARocketPunch::SetMeshVisibility(bool InVisibility)
{
	bStaticMeshVisibility = InVisibility;
	StaticMeshComp->SetVisibility(InVisibility);
}
void ARocketPunch::OnRep_ChangeMeshVisibility()
{
	StaticMeshComp->SetVisibility(bStaticMeshVisibility);
}
void ARocketPunch::OnRep_ChangeScale()
{
	CollisionComp->SetWorldScale3D(CurrentScale);
	StaticMeshComp->SetWorldScale3D(CurrentScale);
}
void ARocketPunch::OnRep_ChangeMesh()
{
	if (CurrentMesh.IsValid())
	{
		StaticMeshComp->SetStaticMesh(CurrentMesh.Get());
	}
}
void ARocketPunch::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != CasterActor) {
		RPCollisionComponent->OnHit(HitComponent,OtherActor,OtherComponent, NormalImpulse, Hit, GetCasterActor(), bIsPush, ForceAlpha);
	}
}
void ARocketPunch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARocketPunch, bStaticMeshVisibility);	
	DOREPLIFETIME_CONDITION(ARocketPunch, CurrentMesh, COND_SimulatedOnly);
	DOREPLIFETIME(ARocketPunch, CurrentScale);
}