// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PassiveItem/PassiveItemRangeUp.h"

void UPassiveItemRangeUp::UseItem(APlayableCharacter* TargetCharacter)
{
    Super::UseItem(TargetCharacter);

    PPLOG(Log, TEXT("UPassiveItemRangeUp is Used!"));

    // @TODO : 속도 증가
}

void UPassiveItemRangeUp::EndActiveItem()
{
    Super::EndActiveItem();

    // @TODO : 속도 감소
}
