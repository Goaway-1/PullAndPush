// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayableController.h"
#include "Blueprint/UserWidget.h"
#include "Widget/InGameHUD.h"
#include "Game/InGameMode.h"
#include "Game/InGameInstance.h"

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
