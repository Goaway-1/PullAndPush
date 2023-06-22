// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LobbyWidget.h"
#include "Components/Button.h"

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::StartGame);
}
void ULobbyWidget::StartGame()
{
    GetWorld()->ServerTravel("/Game/Maps/Level1");
}