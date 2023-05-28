// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ItemData/PassiveItemData/PassiveItemDataRPSizeUp.h"

void UPassiveItemDataRPSizeUp::UsePassiveItem(AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated)
{
    Super::UsePassiveItem(TargetActor, Handler, InPassiveItemAlreadyActivated);

    // Try Rocket Punch Size Up
    if (!bIsItemActivated) {
        CharacterPropertyHandler->SetRocketPunchScale(WeightValue);
    }
}

void UPassiveItemDataRPSizeUp::EndPassiveItem()
{
    Super::EndPassiveItem();

    // Try Rocket Punch Size Down
    CharacterPropertyHandler->SetRocketPunchScale(1.f);
}