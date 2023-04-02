#include "RocketPunch.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "RPMovementComponent.h"

ARocketPunch::ARocketPunch()
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

}
void ARocketPunch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARocketPunch::ReadyToLaunch(const float& Force, AActor* InOwnerPlayerActor, const FVector& InVec, const FRotator& InRot)
{
	RPMovementComponent->Launch(Force, InOwnerPlayerActor, InVec, InRot);
	UE_LOG(LogTemp, Log, TEXT("[ReadyToLaunch] Launch RocketPunch!! %f"), Force);
}