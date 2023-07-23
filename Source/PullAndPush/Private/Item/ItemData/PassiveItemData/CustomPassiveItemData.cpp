#include "Item/ItemData/PassiveItemData/CustomPassiveItemData.h"

void UCustomPassiveItemData::UsePassiveItem(AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated)
{
    Super::UsePassiveItem(TargetActor, Handler, InPassiveItemAlreadyActivated);

    // Try Rocket Punch Speed Up
    if (CharacterPropertyHandler.GetInterface()) 
    {
        if (StatType == ECustomStatType::EST_ItemStat)
        {
            CharacterPropertyHandler->SetPassiveStat(PassiveStat);
        }
        else
        {
            CharacterPropertyHandler->EnableStatFlag(CharacterStat,10.f);
        }
    }
}

void UCustomPassiveItemData::EndPassiveItem()
{
    Super::EndPassiveItem();

    // Try Rocket Punch Speed Down
    if (CharacterPropertyHandler.GetInterface())
    {
        if (StatType == ECustomStatType::EST_ItemStat)
        {
            CharacterPropertyHandler->SetPassiveStat(PassiveStat * -1.f);
        }
        else
        {
            CharacterPropertyHandler->DisableStatFlag(CharacterStat);
        }
    }
}