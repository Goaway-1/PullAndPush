// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PassiveItem/PassiveItemSpeedUp.h"


void UPassiveItemSpeedUp::UseItem(APlayableCharacter* TargetCharacter)
{
    Super::UseItem(TargetCharacter);

    PPLOG(Log, TEXT("UPassiveItemSpeedUp is Used!"));

    // @TODO : �ӵ� ����
}

void UPassiveItemSpeedUp::EndActiveItem()
{
    Super::EndActiveItem();

    // @TODO : �ӵ� ����
}
