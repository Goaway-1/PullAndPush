#include "Item/ItemPickup.h"
#include "Components/SphereComponent.h"
#include "Interface/CharacterPickupHandler.h"
#include "Item/ItemData/ItemData.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"

AItemPickup::AItemPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionProfileName(CollisionName);
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));

	SetRootComponent(CollisionComp);
	NiagaraComp->SetupAttachment(RootComponent);
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
	if (OtherCompCollsionName == "Pawn") 
	{		
		/** Character's Pickup Action */
		TScriptInterface<class ICharacterPickupHandler> ActionHandler = OtherActor;
		if (ActionHandler.GetInterface() && CurItemData.IsValid())
		{
			ActionHandler->PickUpItem(CurItemData.Get());
			SetActiveItemPickup(false);
		}
	}
	else 
	{
		/** Enable Gravity */
		if(!CollisionComp->IsGravityEnabled())
		{
			CollisionComp->SetEnableGravity(true);	
		}
	}
}
void AItemPickup::SetActiveItemPickup(bool IsSpawn, UItemData* InItemDataAsset, FVector InSpawnLocation)
{
	// Turn OnOff Item Enable
	bIsSpawn = IsSpawn;
	CurItemData = InItemDataAsset;

	SetActorEnableCollision(bIsSpawn);
	SetActorHiddenInGame(!bIsSpawn);
	SetActorTickEnabled(bIsSpawn);
	CollisionComp->SetSimulatePhysics(bIsSpawn);			
	CollisionComp->SetEnableGravity(!bIsSpawn);		

	// SetLocation & Mesh if Spawn!
	if(IsSpawn && CurItemData.IsValid())
	{
		CollisionComp->AddForce(FVector::Zero());
		SetActorLocationAndRotation(InSpawnLocation, FRotator::ZeroRotator);
		NiagaraComp->SetAsset(CurItemData.Get()->GetNiagraAsset());
	}
	else if(!IsSpawn)
	{
		// Inform ItemSpawner of Pickup Action.
		OnPickupAction.ExecuteIfBound();
	}
}
void AItemPickup::OnRep_ChangeCurItemData()
{
	CollisionComp->SetSimulatePhysics(bIsSpawn);
	CollisionComp->SetEnableGravity(!bIsSpawn);

	if (CurItemData.IsValid())
	{
		NiagaraComp->SetAsset(CurItemData.Get()->GetNiagraAsset());
	}
}
void AItemPickup::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemPickup, CurItemData);
	DOREPLIFETIME(AItemPickup, bIsSpawn);
}