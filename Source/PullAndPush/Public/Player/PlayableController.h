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
	TObjectPtr<class AInGameHUD> InGameHUD;
#pragma endregion

#pragma region GAMEMODE
public:
	void PlayerFellOutOfWorld(); 

	/** Clear All Timer of Character */
	void ClearAllTimer();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AInGameMode> CurGameMode;
#pragma endregion

<<<<<<< Updated upstream
=======
#pragma region SPECTATE
private:
	/** Set Player to spectate. Should be called only on server */
	UFUNCTION()
	void SetPlayerSpectate();

	/** Notify HUD of a state change so it shows suitable widgets accordingly */
	UFUNCTION(Client, Reliable)
	void ClientHUDStateChanged(EHUDState NewState);

	/** @TODO : 임시 방패용도 (관찰자모드 강제 변경) */
	UFUNCTION()
	void SetState();

#pragma endregion
>>>>>>> Stashed changes
};
