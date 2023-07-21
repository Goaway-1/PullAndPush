#include "Item/ItemSpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "Item/ItemData/ItemData.h"

AItemSpawner::AItemSpawner()
    :
    MaxRespawnDelay(10.f), MinRespawnDelay(3.f)
{
    bReplicates = true;
    SetReplicateMovement(true);

    NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
    NiagaraComp->SetupAttachment(GetRootComponent());

    ItemSpawnType = EItemSpawnType::Normal;
}
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
    InitSetting();
}
void AItemSpawner::InitSetting()
{
    UWorld* const World = GetWorld();
    if (HasAuthority() && World && ItemPickupClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 60.f);
        ItemPickup = World->SpawnActor<AItemPickup>(ItemPickupClass, SpawnLocation, GetActorRotation(), SpawnParams);   // ÀÌ³נ
        ensure(ItemPickup != nullptr);
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
    UItemData* CurItem = SetRandomItemDataAsset();
    ensure(CurItem);
    ItemPickup->SetActiveItemPickup(true, CurItem, SpawnLocation);

    UE_LOG(LogTemp, Log, TEXT("Item Respawn : %s, %s"), *CurItem->GetItemName(), *CurItem->GetItemDescription());
}
UItemData* AItemSpawner::SetRandomItemDataAsset()
{
    // Create an item according to ItemSpawnType
    FPrimaryAssetType PrimaryAssetType = (ItemSpawnType == EItemSpawnType::Active) ? UItemAssetManager::ActiveItemType :
        (ItemSpawnType == EItemSpawnType::Passive) ? UItemAssetManager::PassiveItemType :
        FMath::RandBool() ? UItemAssetManager::ActiveItemType : UItemAssetManager::PassiveItemType;

    // Find Asset using AssetManager
    UAssetManager& Manager = UAssetManager::Get();
    TArray<FPrimaryAssetId> Assets;
    Manager.GetPrimaryAssetIdList(PrimaryAssetType, Assets);
    ensure(0 < Assets.Num());

    // Get Random Item
    int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
    FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
    if (AssetPtr.IsPending())
    {
        AssetPtr.LoadSynchronous();
    }
    return Cast<UItemData>(AssetPtr.Get());
}