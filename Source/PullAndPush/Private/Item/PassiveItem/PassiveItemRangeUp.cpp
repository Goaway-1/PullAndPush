// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PassiveItem/PassiveItemRangeUp.h"

void UPassiveItemRangeUp::UseItem(AActor* TargetActor)
{
    Super::UseItem(TargetActor);

    PPLOG(Log, TEXT("UPassiveItemRangeUp is Used!"));
}

void UPassiveItemRangeUp::EndActiveItem()
{
    Super::EndActiveItem();

    // @TODO : 속도 감소
}
