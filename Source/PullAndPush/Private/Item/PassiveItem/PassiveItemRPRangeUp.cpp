// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PassiveItem/PassiveItemRPRangeUp.h"

void UPassiveItemRPRangeUp::UseItem(AActor* TargetActor)
{
    Super::UseItem(TargetActor);

    // Try Rocket Punch Range Up
    if (!bIsItemActivated) {
        CharacterPropertyHandler->RocketPunchAlphaRange(WeightValue);
    }
}

void UPassiveItemRPRangeUp::EndActiveItem()
{
    Super::EndActiveItem();

    // Try Rocket Punch Range Down
    CharacterPropertyHandler->RocketPunchAlphaRange(1.f);
}
