// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayableController.generated.h"

UCLASS()
class PULLANDPUSH_API APlayableController : public APlayerController
{
	GENERATED_BODY()
public:
	APlayableController();

protected:
	virtual void BeginPlay() override;

#pragma region HUD
public:
	// For Item
	UFUNCTION()
	void UpdateItemUI(class UDataAsset* CurrentItem, const bool IsPassvieItem);

	UFUNCTION()
	void ChangeVisibleItemInfo(bool bVisibility);

	UFUNCTION()
	void UpdateStatUI(const FString& StatName, UMaterialInterface* Material);

private:
	UPROPERTY()
	TObjectPtr<class AMainHUD> MainHUD;
#pragma endregion

#pragma region GAMEMODE
public:
	void PlayerFellOutOfWorld(); 

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AInGameMode> CurGameMode;
#pragma endregion

};
