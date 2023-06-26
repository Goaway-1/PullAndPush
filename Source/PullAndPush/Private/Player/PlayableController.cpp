// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayableController.h"
#include "Blueprint/UserWidget.h"
#include "Widget/InGameHUD.h"
#include "Game/InGameMode.h"

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
		CurGameMode->PlayerFellOutOfWorld(GetName());
	}
}