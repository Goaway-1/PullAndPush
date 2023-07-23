#include "Player/PlayableController.h"
#include "Blueprint/UserWidget.h"
#include "Widget/InGameHUD.h"
#include "Game/InGameMode.h"
#include "Game/InGameInstance.h"
#include "Character/PlayableCharacter.h"

void APlayableController::BeginPlay() 
{
	Super::BeginPlay();

 	InGameHUD = Cast<AInGameHUD>(GetHUD());
	
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
	if (CurGameMode)
	{
		CurGameMode->InitPlayersScore(InPlayerName);
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
void APlayableController::ClientPlayerFellOutOfWorld_Implementation()
{
	UInGameInstance* InGameInstance = Cast<UInGameInstance>(GetGameInstance());
	FString PlayerName = InGameInstance->GetPlayerName().ToString();
	ServerPlayerFellOutOfWorld(PlayerName);

	ServerSetPlayerSpectate();
}
void APlayableController::ServerPlayerFellOutOfWorld_Implementation(const FString& InPlayerName)
{
	CurGameMode = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());
	if (CurGameMode)
	{
		CurGameMode->PlayerFellOutOfWorld(InPlayerName);
	}
}
void APlayableController::ClientInitPlayerCount_Implementation(int8 InTotalPlayerCount)
{
	if (InGameHUD && InGameHUD->InitPlayerCount(InTotalPlayerCount))
	{
		PPLOG(Log,TEXT("InitPlayer Count Successed!"));
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &APlayableController::ClientInitPlayerCount, InTotalPlayerCount));
	}
}	
void APlayableController::ClientSetCurrentPlayerCount_Implementation(int8 InCount)
{
	if (InGameHUD)
	{
		InGameHUD->SetCurrentPlayerCount(InCount);
	}
}
void APlayableController::ClientSetRoundStart_Implementation()
{
	// Check tick-by-tick to see if widget created
	if (InGameHUD && InGameHUD->SetRoundStart())
	{
		PPLOG(Log, TEXT("Set Round Text Successed!"));
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &APlayableController::ClientSetRoundEnd,1.5f,false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlayableController::ClientSetRoundStart);
	}
}
void APlayableController::ClientSetRoundEnd_Implementation()
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
void APlayableController::ServerSetPlayerSpectate_Implementation()
{
	// Only proceed if we're on the server
	if (!HasAuthority()) return;

	// Update the HUD & state
	StartSpectatingOnly();
	ClientHUDStateChanged(EHUDState::Spectating);

	// @TODO : 강제로 진행
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &APlayableController::SetState,1.f,false);
}
void APlayableController::SetState()
{
	ClientGotoState(NAME_Spectating);
}
void APlayableController::ClientHUDStateChanged_Implementation(EHUDState NewState)
{
	if (InGameHUD)
	{
		InGameHUD->OnStateChanged(NewState);
	}
}