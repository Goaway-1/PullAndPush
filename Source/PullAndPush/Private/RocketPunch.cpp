#include "RocketPunch.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPMovementComponent.h"
#include "AttackComponent.h"

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
void ARocketPunch::SetOwner(AActor* OwnActor) {
	OwnerActor = OwnActor;
}
void ARocketPunch::ReadyToLaunch(const float& Force)
{
	if (GetOwnerActor()) {
		RPMovementComponent->ReadyToLaunch(Force);
		UE_LOG(LogTemp, Log, TEXT("Launch RocketPunch!!"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Can't Launch RocketPunch!! OwnerActor is not exist"));
	}
}