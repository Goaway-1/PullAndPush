#include "Item/ItemSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

AItemSpawner::AItemSpawner()
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
    check(ItemPickup)

    UE_LOG(LogTemp, Warning, TEXT("Item Respawn"));
    ItemPickup->SetItemSetting(true, SpawnLocation);
}