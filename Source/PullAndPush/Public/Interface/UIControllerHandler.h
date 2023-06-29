#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UIControllerHandler.generated.h"

UINTERFACE(MinimalAPI)
class UUIControllerHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PULLANDPUSH_API IUIControllerHandler
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void SetCanEditLobbyWidget() = 0;
};
