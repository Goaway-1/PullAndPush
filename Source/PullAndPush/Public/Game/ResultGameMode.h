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

	virtual void StartPlay() override;
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
	UPROPERTY(VisibleAnywhere, Category = "Data")
	int8 TotalPlayerCount;								// @TODO : 로비에서 파악 후 넘겨주어야 함.

	UPROPERTY(VisibleAnywhere, Category = "Data")
	int8 CurrentPlayerCount;
};
