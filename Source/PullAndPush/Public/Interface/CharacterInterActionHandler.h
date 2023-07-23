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
 * Externally applying force to a character
 */
class PULLANDPUSH_API ICharacterInterActionHandler
{
	GENERATED_BODY()

public:
	/** Hit by RocketPunch */
	UFUNCTION()
	virtual void ApplyPunchImpulse(const FVector& DirVec, bool IsPush = true) = 0;

	/** Hit by Obstacle */
	UFUNCTION()
	virtual void ApplyObstacleImpulse(const FVector& DirVec) = 0;
};
