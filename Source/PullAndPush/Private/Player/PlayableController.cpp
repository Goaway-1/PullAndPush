#include "Player/PlayableController.h"
#include "Blueprint/UserWidget.h"
#include "Widget/InGameHUD.h"
#include "Game/InGameMode.h"
#include "Game/InGameInstance.h"
#include "Character/PlayableCharacter.h"
#include "GameFramework/PlayerState.h"

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

	// Set Player Name to GameMode
	if (IsLocalController())
	{
		UInGameInstance* InGameInstance = Cast<UInGameInstance>(GetGameInstance());
		FString PlayerName = InGameInstance->GetPlayerName().ToString();

		ServerSetPlayerNameToMode(PlayerName);
	}
}
void APlayableController::ServerSetPlayerNameToMode_Implementation(const FString& InPlayerName)
{
	CurGameMode = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());
	CurGameMode->InitPlayersScore(InPlayerName);
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
void APlayableController::ClientPlayerFellOutOfWorld_Implementation()
{
	UInGameInstance* InGameInstance = Cast<UInGameInstance>(GetGameInstance());
	FString PlayerName = InGameInstance->GetPlayerName().ToString();
	ServerPlayerFellOutOfWorld(PlayerName);

	SetPlayerSpectate();
}
void APlayableController::ServerPlayerFellOutOfWorld_Implementation(const FString& InPlayerName)
{
	CurGameMode = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());
	CurGameMode->PlayerFellOutOfWorld(InPlayerName);
}
void APlayableController::InitPlayerCount_Implementation(int8 InTotalPlayerCount)
{
	if (InGameHUD && InGameHUD->InitPlayerCount(InTotalPlayerCount))
	{
		PPLOG(Log,TEXT("InitPlayer Count Successed!"));
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &APlayableController::InitPlayerCount, InTotalPlayerCount));
	}
}	
void APlayableController::SetCurrentPlayerCount_Implementation(int8 InCount)
{
	if (InGameHUD)
	{
		InGameHUD->SetCurrentPlayerCount(InCount);
	}
}
void APlayableController::SetRoundStart_Implementation()
{
	// Check tick-by-tick to see if widget created
	if (InGameHUD && InGameHUD->SetRoundStart())
	{
		PPLOG(Log, TEXT("Set Round Text Successed!"));
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &APlayableController::SetRoundEnd,1.5f,false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlayableController::SetRoundStart);
	}
}
void APlayableController::SetRoundEnd_Implementation()
{
	// Enable Input & Hide Widgets
	if (InGameHUD)
	{
		GetPawn()->EnableInput(this);
		InGameHUD->SetRoundEnd();
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