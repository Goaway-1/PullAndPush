// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

UENUM(BlueprintType)
enum class EHUDState : uint8
{
	Playing,
	Spectating,
	Inactive
};

/**
 * InGame level HUD for player
 */
UCLASS()
class PULLANDPUSH_API AInGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UInGameWidget> InGameWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class USpectatorWidget> SpectatorWidgetClass;

	UPROPERTY()
	TObjectPtr<class UInGameWidget> InGameWidget;

	UPROPERTY()
	TObjectPtr<class USpectatorWidget> SpectatorWidget;

/** InGame Widget */
public:
	void UpdateItemUI(UDataAsset* CurrentItem, const bool IsPassvieItem);
	void UpdateStatUI(const FString& StatName, UMaterialInterface* Material);

	void ChangeVisibleItemInfo(bool bVisibility);
/** Player Count Widget */
public:
	// Returns true if widget is created, and saves data for widget
	bool InitPlayerCount(int8 InTotalPlayerCount);
	void SetCurrentPlayerCount(int8 InCount);

	// Returns true if widget is created, and saves data for widget
	bool SetRoundStart();
	void SetRoundEnd();

private:
	/* Current HUD state */
	EHUDState CurrentState;

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	EHUDState GetCurrentState() const;

	/* Event hook to update HUD state (eg. to determine visibility of widgets) */
	UFUNCTION()
	void OnStateChanged(EHUDState NewState);
};
