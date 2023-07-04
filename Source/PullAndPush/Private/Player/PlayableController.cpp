#include "Player/PlayableController.h"
#include "Blueprint/UserWidget.h"
#include "Widget/InGameHUD.h"
#include "Game/InGameMode.h"
#include "Game/InGameInstance.h"
#include "Character/PlayableCharacter.h"

APlayableController::APlayableController() {

}
void APlayableController::BeginPlay() 
{
	Super::BeginPlay();

 	InGameHUD = Cast<AInGameHUD>(GetHUD());
	if (HasAuthority()) 
	{
		CurGameMode = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());
	}
}
void APlayableController::UpdateItemUI(UDataAsset* CurrentItem, const bool IsPassvieItem)
{
	// Set Item Widget
	if (InGameHUD)
	{
		InGameHUD->UpdateItemUI(CurrentItem, IsPassvieItem);
	}
}
void APlayableController::ChangeVisibleItemInfo(bool bVisibility)
{
	if (InGameHUD)
	{
		InGameHUD->ChangeVisibleItemInfo(bVisibility);
	}
}
void APlayableController::UpdateStatUI(const FString& StatName, UMaterialInterface* Material)
{
	if (InGameHUD)
	{
		InGameHUD->UpdateStatUI(StatName,Material);
	}
}
void APlayableController::PlayerFellOutOfWorld()
{
	if (CurGameMode)
	{
		CurGameMode->PlayerFellOutOfWorld(this);
	}

	SetPlayerSpectate();
}
void APlayableController::InitPlayerCount_Implementation(int8 InTotalPlayerCount)
{
	TotalPlayerCount = InTotalPlayerCount;
	SetPlayerCount();
}
void APlayableController::SetPlayerCount()
{
	if (InGameHUD)
	{
		InGameHUD->InitPlayerCount(TotalPlayerCount);
	}
}
void APlayableController::SetCurrentPlayerCount_Implementation(int8 InCount)
{
	if (InGameHUD)
	{
		InGameHUD->SetCurrentPlayerCount(InCount);
	}
}
void APlayableController::ClearAllTimer()
{
	APlayableCharacter* PlayableCharacter = Cast<APlayableCharacter>(GetCharacter());
	if (PlayableCharacter)
	{
		PlayableCharacter->ClearAllTimer();
	}
}
void APlayableController::SetPlayerSpectate()
{
	// Only proceed if we're on the server
	if (!HasAuthority()) return;

	// Update the HUD & state
	StartSpectatingOnly();
	ClientHUDStateChanged(EHUDState::Spectating);

	// @TODO : 임시 방패용도
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &APlayableController::SetState,1.f,false);
}
void APlayableController::SetState()
{
	ClientGotoState(NAME_Spectating);
}
void APlayableController::ClientHUDStateChanged_Implementation(EHUDState NewState)
{
	if (InGameHUD = Cast<AInGameHUD>(GetHUD()))
	{
		InGameHUD->OnStateChanged(NewState);
	}
}
