// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemTimerManager.h"
#include "Item/ItemData/PassiveItemData/PassiveItemData.h"

AItemTimerManager* AItemTimerManager::Instance = nullptr;

AItemTimerManager::AItemTimerManager(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Set the instance to this object
    Instance = this;
}
AItemTimerManager* AItemTimerManager::GetInstance()
{
    return Instance;
}
FTimerHandle AItemTimerManager::AddTimer(const FString& ItemOwnerName, const FString& ItemName, float& Duration, bool bLooping, class UPassiveItemData* PassiveItem, bool& bIsItemActivated)
{   
    // Check handler already exists
    const FName TimerName = FName(ItemOwnerName + '/' + ItemName);
    if (TimerHandles.Contains(TimerName)) 
    {
        RemoveTimer(TimerName);
        PPLOG(Log, TEXT("%s's %s Item Time is already exists!"), *ItemOwnerName, *ItemName);
        bIsItemActivated = true;
    }
    else {
        PPLOG(Log, TEXT("Add Item Timer, Owner : %s, Item Name : %s, Duration : %f, bLooping : %d"), *ItemOwnerName, *ItemName, Duration, bLooping);
        bIsItemActivated = false;
    }

    // Set Timer & Handler
    FTimerHandle Handle;
    FTimerDelegate CallbackDelegate;
    CallbackDelegate.BindLambda([=]() {
        PassiveItem->EndActiveItem();      
        RemoveTimer(TimerName);
    });
    GetWorldTimerManager().SetTimer(Handle, CallbackDelegate, Duration, bLooping);

    // Add the timer handle to the array
    TimerHandles.Add(TimerName, Handle);

    return Handle;
}
void AItemTimerManager::RemoveTimer(FName TimerName)
{
    FTimerHandle* Handle = TimerHandles.Find(TimerName);

    if (Handle->IsValid()) {
        GetWorldTimerManager().ClearTimer(*Handle);
        TimerHandles.Remove(TimerName);

        PPLOG(Log, TEXT("Remove Item Handle : %s"),*TimerName.ToString());
    }
}