// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/PassiveItem/PassiveItemRPSizeUp.h"

void UPassiveItemRPSizeUp::UseItem(AActor* TargetActor)
{
    Super::UseItem(TargetActor);

    // Try Rocket Punch Size Up
    if (!bIsItemActivated) {
        CharacterPropertyHandler->RocketPunchAlphaSize(WeightValue);
    }
}

void UPassiveItemRPSizeUp::EndActiveItem()
{
    Super::EndActiveItem();

    // Try Rocket Punch Size Down
    CharacterPropertyHandler->RocketPunchAlphaSize(1.f);
}