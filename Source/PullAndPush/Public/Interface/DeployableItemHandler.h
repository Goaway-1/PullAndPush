// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeployableItemHandler.generated.h"

UINTERFACE(MinimalAPI)
class UDeployableItemHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * Set DeployableItem State
 */
class PULLANDPUSH_API IDeployableItemHandler
{
	GENERATED_BODY()

public:
	// Set Active or DeActive
	virtual void SetActivePhysicsAndCollision(bool InActive = true) = 0;
};
