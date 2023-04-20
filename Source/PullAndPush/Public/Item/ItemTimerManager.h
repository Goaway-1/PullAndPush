// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "GameFramework/Actor.h"
#include "Item/Item.h"
#include "ItemTimerManager.generated.h"

/**
* Singletone manager manages the timer of the item comprehensively
*/
UCLASS()
class PULLANDPUSH_API AItemTimerManager : public AActor
{
	GENERATED_BODY()

public:
    static AItemTimerManager* GetInstance();    

    UFUNCTION(BlueprintCallable, Category = "Timer Manager")
    void AddTimer(const FString& ItemOwnerName, const FString& ItemName, const EItemActionType ItemType, float& Duration, bool bLooping, class UItemUsageComponent* InUsageComponent);

private:
    AItemTimerManager(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "Timer Manager")
    void RemoveTimer(FName TimerName);

    static AItemTimerManager* Instance;

    UPROPERTY(VisibleAnywhere, Category = "Timer Manager")
    TMap<FName, FTimerHandle> TimerHandles;
};
