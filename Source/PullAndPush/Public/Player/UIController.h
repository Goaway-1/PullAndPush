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

public:
	void ShowResult(TMap<FString, int8>& PlayersScore);

private:
	UFUNCTION(Client, Reliable)
	void ClientShowResult(const TArray<FString>& KeyArray, const TArray<int8>& ValueArray);

private:
	UPROPERTY()
	TObjectPtr<class AResultHUD> ResultHUD;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AResultGameMode> ResultGameMode;
};
