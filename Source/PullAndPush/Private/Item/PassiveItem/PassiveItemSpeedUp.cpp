// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PassiveItem/PassiveItemSpeedUp.h"


void UPassiveItemSpeedUp::UseItem(APlayableCharacter* TargetCharacter)
{
    Super::UseItem(TargetCharacter);

    PPLOG(Log, TEXT("UPassiveItemSpeedUp is Used!"));

    // @TODO : 속도 증가
}

void UPassiveItemSpeedUp::EndActiveItem()
{
    Super::EndActiveItem();

    // @TODO : 속도 감소
}
