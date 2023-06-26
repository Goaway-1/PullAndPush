// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ResultGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API AResultGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AResultGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	/** Start round if all players are logged in to the level */
	void ShowScoreBoard();

private:
	UPROPERTY()
	TObjectPtr<class UInGameInstance> InGameInstance;

	UPROPERTY()
	TArray<class APlayerController*> Controllers;

	// Player Count
	int8 TotalPlayerCount;								
	int8 CurrentPlayerCount;
};
