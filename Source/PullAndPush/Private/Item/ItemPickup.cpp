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
		if (ActionHandler.GetInterface() && CurItem.IsValid()) 
		{
			ActionHandler->PickUpItem(CurItem.Get());

			SetItemSetting(false);

			// Inform ItemSpawner of Pickup Action.
			// @TODO : 나중에 떨어지는 지형과 충돌한 경우 삭제되면 안됌
			OnPickupAction.Execute();
		}
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
void AItemPickup::SetItemSetting(bool IsSpawn, UItemData* InItemDataAsset, FVector SpawnLocation)
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
}

