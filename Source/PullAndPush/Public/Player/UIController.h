#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UIController.generated.h"

/**
 * Controllers used only in the UI
 */
UCLASS()
class PULLANDPUSH_API AUIController : public APlayerController
{
	GENERATED_BODY()

public:
	AUIController();

protected:
	virtual void BeginPlay() override;
};
