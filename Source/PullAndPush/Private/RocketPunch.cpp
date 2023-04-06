#include "RocketPunch.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "RPMovementComponent.h"
#include "RPCollisionComponent.h"

ARocketPunch::ARocketPunch()
	: 
	bIsPush(false)
{
 	PrimaryActorTick.bCanEverTick = true;

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

	RPMovementComponent = CreateDefaultSubobject<URPMovementComponent>(TEXT("RPMovementComponent"));
	RPCollisionComponent = CreateDefaultSubobject<URPCollisionComponent>(TEXT("RPCollisionComponent"));
}
void ARocketPunch::BeginPlay()
{
	Super::BeginPlay();

	// Set Overlap Collision
	CollisionComp->OnComponentHit.AddDynamic(this, &ARocketPunch::OnHit);

	// Set RocketPunch Force Return
	RPCollisionComponent->OnForceReturn.BindUObject(RPMovementComponent, &URPMovementComponent::SetIsForceReturn);
}
void ARocketPunch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARocketPunch::ReadyToLaunch(const float& Force, AActor* InOwnerPlayerActor, const bool IsPush, const FVector& InVec, const FRotator& InRot)
{
	if(OwnerPlayerActor == nullptr) OwnerPlayerActor = InOwnerPlayerActor;

	bIsPush = IsPush;
	RPMovementComponent->Launch(Force, OwnerPlayerActor, InVec, InRot);
	
	// For Log
	// TODO : 추후 색의 차이가 아닌, 새로운 매시로 구분하고 함수로 제작
	if(!PushMaterial || !PullMaterial) {
		UE_LOG(LogTemp, Warning, TEXT("[RocketPunch] Materials ared not exsit"));
		return;
	}

	FString AttackType = (bIsPush) ? TEXT("PUSH") : TEXT("PULL");
	UMaterial* CurMaterial;

	if (bIsPush)
	{
		AttackType = TEXT("PUSH");
		CurMaterial = PushMaterial;
	}
	else
	{
		AttackType = TEXT("PULL");
		CurMaterial = PullMaterial;
	}

	UE_LOG(LogTemp, Log, TEXT("[ARocketPunch] %s RocketPunch!! %f"), *AttackType, Force);
	StaticMeshComp->SetMaterial(0, CurMaterial);
}
void ARocketPunch::SetCollisionSimulatePhysics(bool Val)
{
	if(CollisionComp) CollisionComp->SetSimulatePhysics(Val);
}
void ARocketPunch::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != OwnerPlayerActor) {
		RPCollisionComponent->OnHit(HitComponent,OtherActor,OtherComponent, NormalImpulse,Hit, bIsPush);
	}
}