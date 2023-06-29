// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/PlayableControllerHandler.h"
#include "PlayableController.generated.h"

UCLASS()
class PULLANDPUSH_API APlayableController : public APlayerController, public IPlayableControllerHandler
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

	UFUNCTION(Client, Reliable)
	void InitPlayerCount(int8 InTotalPlayerCount);

	UFUNCTION(Client, Reliable)
	void SetCurrentPlayerCount(int8 InCount);

	virtual void SetPlayerCount() override;
private:
	UPROPERTY()
	TObjectPtr<class AInGameHUD> InGameHUD;

	int8 TotalPlayerCount;
#pragma endregion

#pragma region GAMEMODE
public:
	void PlayerFellOutOfWorld(); 

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AInGameMode> CurGameMode;
#pragma endregion
};
