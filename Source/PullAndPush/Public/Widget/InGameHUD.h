// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

<<<<<<< Updated upstream
/**
 * 
 */
=======
UENUM(BlueprintType)
enum class EHUDState : uint8
{
	Playing,
	Spectating,
	Inactive
};

>>>>>>> Stashed changes
UCLASS()
class PULLANDPUSH_API AInGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UInGameWidget> InGameWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class USpectatorWidget> SpectatorWidgetClass;

	UPROPERTY()
	TObjectPtr<class UInGameWidget> InGameWidget;

<<<<<<< Updated upstream
/** Funtion */
=======
	UPROPERTY()
	TObjectPtr<class USpectatorWidget> SpectatorWidget;

/** InGame Widget */
>>>>>>> Stashed changes
public:
	void UpdateItemUI(UDataAsset* CurrentItem, const bool IsPassvieItem);

<<<<<<< Updated upstream
	void ChangeVisibleItemInfo(bool bVisibility);

	void UpdateStatUI(const FString& StatName, UMaterialInterface* Material);
=======
/** Player Count Widget */
public:
	void InitPlayerCount(int8 InTotalPlayerCount);
	void SetCurrentPlayerCount(int8 InCount);

	// @TEST
private:
	/* Current HUD state */
	EHUDState CurrentState;

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	EHUDState GetCurrentState() const;

	/* Event hook to update HUD state (eg. to determine visibility of widgets) */
	UFUNCTION()
	void OnStateChanged(EHUDState NewState);
>>>>>>> Stashed changes
};
