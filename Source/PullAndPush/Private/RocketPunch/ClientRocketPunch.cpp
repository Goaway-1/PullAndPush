#include "RocketPunch/RocketPunch.h"
#include "RocketPunch/RPMovementComponent.h"
#include "Interface/CharacterStatHandler.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

AClientRocketPunch::AClientRocketPunch()
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
}
void AClientRocketPunch::BeginPlay()
{
	Super::BeginPlay();

	ensure(PushMesh);
	ensure(PullMesh);
}
void AClientRocketPunch::ReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const bool InForceVisibility, const FVector& InVec, const FRotator& InRot, const float& DeltaSpeed, const float& DeltaRange, const float& DeltaScale)
{
	if (!HasAuthority())
	{
		ServerReadyToLaunch(InForceAlpha, InCasterActor, IsPush, InForceVisibility, InVec, InRot, DeltaSpeed, DeltaRange, DeltaScale);
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
		RPMovementComponent->Launch(ForceAlpha, CasterActor.Get(), InForceVisibility, InVec, InRot, DeltaSpeed, DeltaRange);
		PPLOG(Log, TEXT("AlphaSpeed : %f, AlphaRange : %f, AlphaSize : %s"), DeltaSpeed, DeltaRange, *CurrentScale.ToString());

		// @TODO : ÀÌ½´ Á¸Àç : Setting Static mesh of RP
		CurrentMesh = (bIsPush) ? PushMesh : PullMesh;
		//OnRep_ChangeMesh();
	}
}
void AClientRocketPunch::ServerReadyToLaunch_Implementation(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const bool InForceVisibility, const FVector& InVec, const FRotator& InRot, const float& DeltaSpeed, const float& DeltaRange, const float& DeltaScale)
{
	ReadyToLaunch(InForceAlpha, InCasterActor, IsPush, InForceVisibility, InVec, InRot, DeltaSpeed, DeltaRange, DeltaScale);
}
void AClientRocketPunch::SetCollisionSimulatePhysics(bool Val)
{
	if (CollisionComp) CollisionComp->SetSimulatePhysics(Val);
}
void AClientRocketPunch::SetMeshVisibility(bool InVisibility)
{
	bStaticMeshVisibility = InVisibility;
	StaticMeshComp->SetVisibility(InVisibility);
}
void AClientRocketPunch::OnRep_ChangeMeshVisibility()
{
	StaticMeshComp->SetVisibility(bStaticMeshVisibility);
}
void AClientRocketPunch::OnRep_ChangeScale()
{
	CollisionComp->SetWorldScale3D(CurrentScale);
	StaticMeshComp->SetWorldScale3D(CurrentScale);
}
void AClientRocketPunch::SetForceReturn()
{
	PPLOG(Warning, TEXT("OnForceReturn"));
	RPMovementComponent->SetIsForceReturn(true);
}
void AClientRocketPunch::OnRep_ChangeMesh()
{
	if (CurrentMesh.IsValid())
	{
		StaticMeshComp->SetStaticMesh(CurrentMesh.Get());
	}
}
void AClientRocketPunch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AClientRocketPunch, bStaticMeshVisibility);
	DOREPLIFETIME(AClientRocketPunch, CurrentMesh);
	DOREPLIFETIME(AClientRocketPunch, CurrentScale);
}