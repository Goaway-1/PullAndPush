#include "Item/ItemPickup.h"
#include "Components/SphereComponent.h"
#include "Interface/PickupActionHandler.h"
#include "Item/Item.h"

AItemPickup::AItemPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));

	SetRootComponent(CollisionComp);
	StaticMeshComp->SetupAttachment(RootComponent);
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
		UE_LOG(LogTemp, Log, TEXT("Item overrlap with Pawn"));
		
		/** Character's Pickup Action */
		TScriptInterface<class IPickupActionHandler> ActionHandler = OtherActor;
		if (ActionHandler.GetInterface() && CurItem) 
		{
			ActionHandler->PickUpItem(CurItem);
		}

		SetItemSetting(false);

		// Inform ItemSpawner of Pickup Action.
		// @TODO : 나중에 떨어지는 지형과 충돌한 경우에도 알려야 함.
		OnPickupAction.Execute();		
	}
	else if(OtherCompCollsionName == "RocketPunch") {
		UE_LOG(LogTemp, Log, TEXT("Item overrlap with Punch"));
		
		// Enable Gravity
		if(!CollisionComp->IsGravityEnabled())
		{
			CollisionComp->SetEnableGravity(true);	
		}
	}
}
void AItemPickup::SetItemSetting(bool IsSpawn, UItem* InItemDataAsset, FVector SpawnLocation)
{
	// Turn OnOff Actor Enable
	SetActorEnableCollision(IsSpawn);
	SetActorHiddenInGame(!IsSpawn);
	SetActorTickEnabled(IsSpawn);
	CollisionComp->SetSimulatePhysics(IsSpawn);
	CollisionComp->SetEnableGravity(false);
	CurItem = InItemDataAsset;

	// SetLocation & Mesh if Spawn!
	if(IsSpawn && CurItem)
	{
		SetActorLocationAndRotation(SpawnLocation, FRotator::ZeroRotator);
		StaticMeshComp->SetStaticMesh(CurItem->GetStaticMesh());
		StaticMeshComp->SetRelativeLocation(FVector::Zero());
	}
}

