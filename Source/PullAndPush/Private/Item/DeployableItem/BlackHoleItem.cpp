#include "Item/DeployableItem/BlackHoleItem.h"
#include "Components/SphereComponent.h"
#include "Interface/ItemPickupPropertyHandler.h"

ABlackHoleItem::ABlackHoleItem()
	: 
	ForceStength(10000.f),bIsBlackHoleActived(0)
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(GetRootComponent());
	SphereComp->SetSphereRadius(1500.f);
}
void ABlackHoleItem::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&ABlackHoleItem::AddOverlapActors);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ABlackHoleItem::DeleteOverlapActors);
}
void ABlackHoleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set Acitve Black Hole
	if (bIsBlackHoleActived) 
	{
		ActivateBlackHole();
	}
}
void ABlackHoleItem::ActiveDeployableItem()
{
	Super::ActiveDeployableItem();

	bIsBlackHoleActived = true;
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetEnableGravity(false);
}
void ABlackHoleItem::ActivateBlackHole()
{
	// Activated Black Hole Gravity
	for (auto PrimitiveComp : OverlapComponent)
	{
		const FVector BlackHoleLocation = GetActorLocation();
		if (PrimitiveComp.IsValid()) {
			// Set Size according to distance
			FVector PrimitiveCompLocation = PrimitiveComp.Get()->GetComponentLocation();
			const float Distance = FVector::Distance(PrimitiveCompLocation, BlackHoleLocation);
			const float ForceMagnitude = ForceStength / FMath::Max(Distance, 1.f);

			// Get Direction vector
			FVector FroceVec = BlackHoleLocation - PrimitiveCompLocation;
			FroceVec.Normalize();

			// Apply Force
			PrimitiveComp.Get()->SetPhysicsLinearVelocity(FroceVec * ForceMagnitude, true);
		}
	}
}
void ABlackHoleItem::AddOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsSimulatingPhysics())
	{
		OverlapComponent.Add(OtherComp);
		
		// Exception handling if item
		TScriptInterface<class IItemPickupPropertyHandler> ActionHandler = OtherComp;
		if (ActionHandler.GetInterface()) {
			ActionHandler->SetActiveItemPickup(true);
		}
	}
}
void ABlackHoleItem::DeleteOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlapComponent.Remove(OtherComp);
}
