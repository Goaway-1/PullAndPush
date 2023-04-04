#include "RocketPunch.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "RPMovementComponent.h"

ARocketPunch::ARocketPunch()
	: 
	bIsPush(false)
{
 	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	SetRootComponent(CollisionComp);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(GetRootComponent());

	RPMovementComponent = CreateDefaultSubobject<URPMovementComponent>(TEXT("RPMovementComponent"));
}
void ARocketPunch::BeginPlay()
{
	Super::BeginPlay();

	// Set Overlap Collision
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARocketPunch::OnOverlapBegin);
}
void ARocketPunch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARocketPunch::ReadyToLaunch(const float& Force, AActor* InOwnerPlayerActor, const bool& IsPush, const FVector& InVec, const FRotator& InRot)
{
	if(OwnerPlayerActor == nullptr) OwnerPlayerActor = InOwnerPlayerActor;

	bIsPush = IsPush;
	RPMovementComponent->Launch(Force, OwnerPlayerActor, InVec, InRot);
	
	// LOG
	FString AttackType = (bIsPush) ? TEXT("PUSH") : TEXT("PULL");
	UE_LOG(LogTemp, Log, TEXT("[ARocketPunch] %s RocketPunch!! %f"), *AttackType, Force);
}
void ARocketPunch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor != OwnerPlayerActor) {
		UE_LOG(LogTemp, Log, TEXT("[ARocketPunch] Overrlap Type is '%s', Name is '%s'"), *OtherComp->GetCollisionProfileName().ToString(), *OtherActor->GetName());
	}
}
