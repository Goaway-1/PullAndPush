#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RocketPunchHandler.generated.h"

UINTERFACE(MinimalAPI)
class URocketPunchHandler : public UInterface
{
	GENERATED_BODY()
};

class PULLANDPUSH_API IRocketPunchHandler
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void SetCollisionSimulatePhysics(bool Val) = 0;
	
	UFUNCTION()
	virtual void SetMeshVisibility(bool InVisibility) = 0;
};
