// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PassiveItem/PassiveItemRPSpeedUp.h"

void UPassiveItemRPSpeedUp::UseItem(AActor* TargetActor)
{
    Super::UseItem(TargetActor);

    // Try Rocket Punch Speed Up
    if (!bIsItemActivated) {
        CharacterPropertyHandler->RocketPunchAlphaSpeed(WeightValue);
    }
}

void UPassiveItemRPSpeedUp::EndActiveItem()
{
    Super::EndActiveItem();

    // Try Rocket Punch Speed Down
    CharacterPropertyHandler->RocketPunchAlphaSpeed(1.f);
}