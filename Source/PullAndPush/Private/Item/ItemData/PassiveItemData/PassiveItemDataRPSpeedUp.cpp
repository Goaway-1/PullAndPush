// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemData/PassiveItemData/PassiveItemDataRPSpeedUp.h"

void UPassiveItemDataRPSpeedUp::UsePassiveItem(AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated)
{
    Super::UsePassiveItem(TargetActor, Handler, InPassiveItemAlreadyActivated);

    // Try Rocket Punch Speed Up
    if (!bIsItemActivated) {
        CharacterPropertyHandler->RocketPunchAlphaSpeed(WeightValue);
    }
}

void UPassiveItemDataRPSpeedUp::EndPassiveItem()
{
    Super::EndPassiveItem();

    // Try Rocket Punch Speed Down
    CharacterPropertyHandler->RocketPunchAlphaSpeed(1.f);
}