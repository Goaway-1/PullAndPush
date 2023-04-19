// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/PickupActionHandler.h"
#include "ItemUsageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PULLANDPUSH_API UItemUsageComponent : public UActorComponent, public IPickupActionHandler
{
	GENERATED_BODY()

public:	
	UItemUsageComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SpeedUp(class UItem* ItemData) override;
};
