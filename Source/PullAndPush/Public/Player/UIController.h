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

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	uint8 bIsResultController:1;

#pragma region MAIN/LOBBY


#pragma endregion

#pragma region RESULT
public:
	void ShowResult(TMap<FString, int8>& PlayersScore);

private:
	UFUNCTION(Client, Reliable)
	void ClientShowResult(const TArray<FString>& KeyArray, const TArray<int8>& ValueArray);

private:
	UPROPERTY()
	TWeakObjectPtr<class AResultHUD> ResultHUD;
#pragma endregion
};
