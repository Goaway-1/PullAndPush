// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SpectatorWidget.h"
#include "Components/TextBlock.h"

void USpectatorWidget::InitPlayerCount(int8 InTotalPlayerCount)
{
    TotalCount->SetText(FText::AsNumber(InTotalPlayerCount));
    CurrentCount->SetText(FText::AsNumber(InTotalPlayerCount));
}
void USpectatorWidget::SetCurrentPlayerCount(int8 InCount)
{
    CurrentCount->SetText(FText::AsNumber(InCount));
}
