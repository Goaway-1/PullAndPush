// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterActionHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterInterActionHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PULLANDPUSH_API ICharacterInterActionHandler
{
	GENERATED_BODY()

public:
	/** Hit by push */
	UFUNCTION()
	virtual void KnockBackActor(const FVector& DirVec) = 0;

	/** pull */
	UFUNCTION()
	virtual void SetMoveToLocation(const FVector& HitVector) = 0;

	/** Hit by pull */
	UFUNCTION()
	virtual void SetMoveToActor(AActor* TargetActor) = 0;
};
