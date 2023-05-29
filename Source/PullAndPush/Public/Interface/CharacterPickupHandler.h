// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterPickupHandler.generated.h"

UINTERFACE(MinimalAPI)
class UCharacterPickupHandler : public UInterface
{
	GENERATED_BODY()
};

class PULLANDPUSH_API ICharacterPickupHandler
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void PickUpItem(class UItemData* ItemData) = 0;
};
