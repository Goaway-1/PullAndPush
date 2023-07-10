#include "Item/ItemData/PassiveItemData/CustomPassiveItemData.h"

void UCustomPassiveItemData::UsePassiveItem(AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated)
{
    Super::UsePassiveItem(TargetActor, Handler, InPassiveItemAlreadyActivated);

    // Try Rocket Punch Speed Up
    if (CharacterPropertyHandler.GetInterface()) {
        CharacterPropertyHandler->SetPassiveStat(PassiveStat);
    }
}

void UCustomPassiveItemData::EndPassiveItem()
{
    Super::EndPassiveItem();

    // Try Rocket Punch Speed Down
    if (CharacterPropertyHandler.GetInterface())
    {
        CharacterPropertyHandler->SetPassiveStat(PassiveStat * -1.f);
    }
}