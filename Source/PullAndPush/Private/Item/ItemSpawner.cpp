#include "Item/ItemSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Item/Item.h"

AItemSpawner::AItemSpawner()
    :
    MaxRespawnDelay(10.f), MinRespawnDelay(3.f)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
}
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

    InitSetting();
}
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AItemSpawner::InitSetting()
{
    UWorld* const World = GetWorld();
    if (World && ItemPickupClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 50.f);
        ItemPickup = World->SpawnActor<AItemPickup>(ItemPickupClass, SpawnLocation, GetActorRotation(), SpawnParams);
        RespawnItem();

        // Respawn after invoking item hit event
        ItemPickup->OnPickupAction.BindUObject(this, &AItemSpawner::TryRespawnItem);
    }
}
void AItemSpawner::TryRespawnItem()
{
    const float SpawnDelay = UKismetMathLibrary::RandomFloatInRange(MinRespawnDelay,MaxRespawnDelay);

    UE_LOG(LogTemp, Warning, TEXT("Respawn Item after %f times"), SpawnDelay);
    GetWorld()->GetTimerManager().ClearTimer(RespawnHandle);
    GetWorld()->GetTimerManager().SetTimer(RespawnHandle, this, &AItemSpawner::RespawnItem, SpawnDelay, false);
}
void AItemSpawner::RespawnItem() {
    ensure(ItemPickup);

    // Set Item Data
    UItem* CurItem = SetRandomItemDataAsset();
    ensure(CurItem);
    ItemPickup->SetItemSetting(true, CurItem, SpawnLocation);

    UE_LOG(LogTemp, Log, TEXT("Item Respawn : %s, %s"), *CurItem->Name, *CurItem->Description);
}
UItem* AItemSpawner::SetRandomItemDataAsset()
{
    if (ItemDataArray.Num() <= 0) {
        UE_LOG(LogTemp,Warning,TEXT("ItemDataArray is nullptr"));
        return nullptr;
    }

    const int8 ItemIndex = UKismetMathLibrary::RandomFloatInRange(0, ItemDataArray.Num());
    return ItemDataArray[ItemIndex];
}