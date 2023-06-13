#include "Gimmick/PhyiscsObstacle.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

APhyiscsObstacle::APhyiscsObstacle()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComp"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	PhysicsConstraintComp = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintComp"));

	SetRootComponent(DefaultSceneComp);
	StaticMeshComp->SetupAttachment(GetRootComponent());
	StaticMeshComp->SetSimulatePhysics(true);
	PhysicsConstraintComp->SetupAttachment(GetRootComponent());
}