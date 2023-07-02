// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemData/PassiveItemData/PassiveItemDataRPRangeUp.h"

void UPassiveItemDataRPRangeUp::UsePassiveItem(AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated)
{
    Super::UsePassiveItem(TargetActor, Handler, InPassiveItemAlreadyActivated);

    // Try Rocket Punch Range Up
    if (CharacterPropertyHandler.GetInterface() && !bIsItemActivated) {
        CharacterPropertyHandler->SetRocketPunchRange(WeightValue);
    }
}

void UPassiveItemDataRPRangeUp::EndPassiveItem()
{
    Super::EndPassiveItem();

    // Try Rocket Punch Range Down
    if (CharacterPropertyHandler.GetInterface())
    {
        CharacterPropertyHandler->SetRocketPunchRange(1.f);
    }
}
