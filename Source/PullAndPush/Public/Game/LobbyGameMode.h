#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	void SetLobbyWidgetData(APlayerController* NewPlayer);

private:
	UPROPERTY()
	TObjectPtr<class UInGameInstance> InGameInstance;

	UPROPERTY()
	TArray<class AUIController*> Controllers;
};
