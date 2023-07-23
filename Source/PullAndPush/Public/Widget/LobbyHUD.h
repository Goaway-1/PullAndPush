// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 * Lobby level HUD for player
 */
UCLASS()
class PULLANDPUSH_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	// Returns true if widget is created, and saves data for widget
	bool SetLobbyWidgetData(int8 InMaxPlayerCount, int8 InTotalPlayerCount);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class ULobbyWidget> LobbyWidgetClass;

	UPROPERTY()
	TObjectPtr<class ULobbyWidget> LobbyWidget;
};
