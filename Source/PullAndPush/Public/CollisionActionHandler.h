// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CollisionActionHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCollisionActionHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PULLANDPUSH_API ICollisionActionHandler
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void KnockBackActor(const FVector& DirVec) = 0;

	UFUNCTION()
	virtual void SetMoveToLocation(const FVector& HitVector) = 0;
};
