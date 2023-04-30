#include "RocketPunch/RocketPunch.h"
#include "RocketPunch/RPMovementComponent.h"
#include "RocketPunch/RPCollisionComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"

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

	// Set Weapon Out of use!
	RPMovementComponent->OnReturn.BindUObject(this, &ARocketPunch::IsOutOfUse);

	// Set RocketPunch Force Return
	RPCollisionComponent->OnForceReturn.BindUObject(RPMovementComponent, &URPMovementComponent::SetIsForceReturn);
}
void ARocketPunch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARocketPunch::ReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, const float& AlphaSpeed, const float& AlphaRange, const float& AlphaSize)
{
	if(CasterActor == nullptr) CasterActor = InCasterActor;

	// Set Scale of Visual & Collision
	CollisionComp->SetWorldScale3D(FVector(AlphaSize));
	StaticMeshComp->SetWorldScale3D(FVector(AlphaSize));

	bIsPush = IsPush;
	ForceAlpha = InForceAlpha;
	RPMovementComponent->Launch(ForceAlpha, CasterActor, InVec, InRot, AlphaSpeed, AlphaSize);
	PPLOG(Log, TEXT("AlphaSpeed : %f, AlphaRange : %f, AlphaSize : %f"), AlphaSpeed, AlphaRange, AlphaSize);
	
	// Setting Color of RP
	// @TODO : ���� ���� ���̰� �ƴ�, ���ο� �Ž÷� �����ϰ� �Լ��� ����
	if(!PushMaterial || !PullMaterial) {
		UE_LOG(LogTemp, Warning, TEXT("[RocketPunch] Materials ared not exsit"));
		return;
	}

	UMaterial* CurMaterial = (bIsPush) ? PushMaterial : PullMaterial;
	StaticMeshComp->SetMaterial(0, CurMaterial);
}
void ARocketPunch::IsOutOfUse(const bool& Val)
{
	OutOfUse.Execute(Val);

	// Reset OverlapActors Array
	RPCollisionComponent->OnArrayReset.Execute();
}
AActor* ARocketPunch::GetCasterActor()
{
	if(CasterActor) return CasterActor;
	return nullptr;
}
void ARocketPunch::SetCollisionSimulatePhysics(bool Val)
{
	if(CollisionComp) CollisionComp->SetSimulatePhysics(Val);
}
void ARocketPunch::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != CasterActor) {
		RPCollisionComponent->OnHit(HitComponent,OtherActor,OtherComponent, NormalImpulse, Hit, GetCasterActor(), bIsPush, ForceAlpha);
	}
}