// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayableController.h"
#include "Blueprint/UserWidget.h"
#include "Widget/MainWidget.h"

APlayableController::APlayableController() {

}
void APlayableController::BeginPlay() {
	ensure(MainWidgetClass);

	MainWidget = CreateWidget<UMainWidget>(this, MainWidgetClass);
	if (MainWidget) {
		MainWidget->AddToViewport();
		MainWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
void APlayableController::UpdateItemUI(UDataAsset* CurrentItem, const bool& IsPassvieItem)
{
	ensure(MainWidget);

	// Set Item Widget
	MainWidget->UpdateItemUI(CurrentItem, IsPassvieItem);
}
void APlayableController::ChangeVisibleItemInfo(bool bVisibility)
{
	MainWidget->OnChangeVisibleItemWidget.Execute(bVisibility);
}
