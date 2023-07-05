#include "RocketPunch/RocketPunch.h"
#include "RocketPunch/RPMovementComponent.h"
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
	CollisionComp->InitSphereRadius(0.f);
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetEnableGravity(false);
	CollisionComp->SetNotifyRigidBodyCollision(true);
	CollisionComp->SetGenerateOverlapEvents(false);
	SetRootComponent(CollisionComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetWorldScale3D(FVector(0.3f));
	StaticMeshComp->SetCollisionProfileName(NoCollisionName);
	StaticMeshComp->SetupAttachment(GetRootComponent());
	StaticMeshComp->SetIsReplicated(true);

	RPMovementComponent = CreateDefaultSubobject<URPMovementComponent>(TEXT("RPMovementComponent"));
}
void AClientRocketPunch::BeginPlay()
{
	Super::BeginPlay();

	ensure(PushMesh);
	ensure(PullMesh);

	SetMeshVisibility(false);
}
void AClientRocketPunch::ReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, FPassiveStat InPassiveStat)
{
	if (!HasAuthority())
	{
		ServerReadyToLaunch(InForceAlpha, InCasterActor, IsPush, InVec, InRot, InPassiveStat);
	}
	else
	{
		if (!CasterActor.IsValid()) CasterActor = InCasterActor;

		// Set Scale of Visual & Collision
		CurrentPassiveStat = InPassiveStat;
		CurrentScale = (InPassiveStat.RPScale < KINDA_SMALL_NUMBER) ? FVector(1.f) : FVector(InPassiveStat.RPScale);
		CollisionComp->SetWorldScale3D(CurrentScale);
		StaticMeshComp->SetWorldScale3D(CurrentScale);

		bIsPush = IsPush;
		ForceAlpha = InForceAlpha;
		RPMovementComponent->Launch(ForceAlpha, CasterActor.Get(), InVec, InRot, InPassiveStat);
		PPLOG(Log, TEXT("AlphaSpeed : %f, AlphaRange : %f, AlphaSize : %s"), InPassiveStat.RPSpeed, InPassiveStat.RPRange, *CurrentScale.ToString());

		// @TODO : ÀÌ½´ Á¸Àç : Setting Static mesh of RP
		SetMeshChange(bIsPush);
	}
}
void AClientRocketPunch::ServerReadyToLaunch_Implementation(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, FPassiveStat InPassiveStat)
{
	ReadyToLaunch(InForceAlpha, InCasterActor, IsPush, InVec, InRot, InPassiveStat);
}
void AClientRocketPunch::SetForceReturn()
{
	RPMovementComponent->SetIsForceReturn(true);
}
void AClientRocketPunch::SetCollisionSimulatePhysics(bool Val)
{
	if (CollisionComp) CollisionComp->SetSimulatePhysics(Val);
}
void AClientRocketPunch::SetMeshVisibility(bool InVisibility)
{
	StaticMeshComp->SetVisibility(InVisibility);
}
void AClientRocketPunch::SetMeshChange(bool IsPush)
{
	UStaticMesh* NewMesh = (IsPush) ? PushMesh : PullMesh;
	StaticMeshComp->SetStaticMesh(NewMesh);
}
void AClientRocketPunch::OnRep_ChangeScale()
{
	CollisionComp->SetWorldScale3D(CurrentScale);
	StaticMeshComp->SetWorldScale3D(CurrentScale);
}
void AClientRocketPunch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AClientRocketPunch, CurrentScale);
}