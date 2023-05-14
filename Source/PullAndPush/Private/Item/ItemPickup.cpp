#include "Item/ItemPickup.h"
#include "Components/SphereComponent.h"
#include "Interface/PickupActionHandler.h"
#include "Item/ItemData/ItemData.h"

AItemPickup::AItemPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionProfileName(CollisionName);
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));

	SetRootComponent(CollisionComp);
	StaticMeshComp->SetupAttachment(RootComponent);
}
void AItemPickup::FellOutOfWorld(const UDamageType& dmgType)
{
	PPLOG(Log, TEXT("Item FellOutOfWorld"));
	SetActiveItemPickup(false);
}
void AItemPickup::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentHit.AddDynamic(this, &AItemPickup::OnHit);
}
void AItemPickup::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	const FName OtherCompCollsionName = OtherComponent->GetCollisionProfileName();

	// Hit Event of Push & Pull or Character
	if (OtherCompCollsionName == "Pawn") {		
		/** Character's Pickup Action */
		TScriptInterface<class IPickupActionHandler> ActionHandler = OtherActor;
		if (ActionHandler.GetInterface() && CurItem.IsValid()) 
		{
			ActionHandler->PickUpItem(CurItem.Get());
			SetActiveItemPickup(false);
		}
	}
	else {
		/** Enable Gravity */
		if(!CollisionComp->IsGravityEnabled())
		{
			CollisionComp->SetEnableGravity(true);	
		}
	}
}
void AItemPickup::SetActiveItemPickup(bool IsSpawn, UItemData* InItemDataAsset, FVector SpawnLocation)
{
	// Turn OnOff Actor Enable
	SetActorEnableCollision(IsSpawn);
	SetActorHiddenInGame(!IsSpawn);
	SetActorTickEnabled(IsSpawn);
	CollisionComp->SetSimulatePhysics(IsSpawn);
	CollisionComp->SetEnableGravity(false);
	CurItem = InItemDataAsset;

	// SetLocation & Mesh if Spawn!
	if(IsSpawn && CurItem.IsValid())
	{
		SetActorLocationAndRotation(SpawnLocation, FRotator::ZeroRotator);
		StaticMeshComp->SetStaticMesh(CurItem.Get()->GetStaticMesh());
		StaticMeshComp->SetRelativeLocation(FVector::Zero());
	}
	else
	{
		// Inform ItemSpawner of Pickup Action.
		OnPickupAction.Execute();
	}
}