// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayableController.h"
#include "Blueprint/UserWidget.h"
#include "Widget/MainHUD.h"

APlayableController::APlayableController() {

}
void APlayableController::BeginPlay() 
{
	Super::BeginPlay();

	MainHUD = Cast<AMainHUD>(GetHUD());
}
void APlayableController::UpdateItemUI(UDataAsset* CurrentItem, const bool IsPassvieItem)
{
	// Set Item Widget
	if (MainHUD)
	{
		MainHUD->UpdateItemUI(CurrentItem, IsPassvieItem);
	}
}
void APlayableController::ChangeVisibleItemInfo(bool bVisibility)
{
	if (MainHUD)
	{
		MainHUD->ChangeVisibleItemInfo(bVisibility);
	}
}
void APlayableController::UpdateStatUI(const FString& StatName, UMaterialInterface* Material)
{
	if (MainHUD)
	{
		MainHUD->UpdateStatUI(StatName,Material);
	}
}