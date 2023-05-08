// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemData/PassiveItemData/PassiveItemDataRPRangeUp.h"

void UPassiveItemDataRPRangeUp::UseItem(AActor* TargetActor)
{
    Super::UseItem(TargetActor);

    // Try Rocket Punch Range Up
    if (!bIsItemActivated) {
        CharacterPropertyHandler->RocketPunchAlphaRange(WeightValue);
    }
}

void UPassiveItemDataRPRangeUp::EndActiveItem()
{
    Super::EndActiveItem();

    // Try Rocket Punch Range Down
    CharacterPropertyHandler->RocketPunchAlphaRange(1.f);
}
