// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PassiveItemWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"


void UPassiveItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Set Widget Value..
	const float RemainTime = UKismetSystemLibrary::K2_GetTimerRemainingTimeHandle(GetWorld(), ItemTimerHandler);
	Timer->SetText(FText::AsNumber(RemainTime));

	const float Parcent = RemainTime / ItemCost;
	ItemMaterial->SetScalarParameterValue(PercentageParameterName, Parcent);
	
	if (RemainTime < KINDA_SMALL_NUMBER)
	{
		RemoveFromParent();
		ConditionalBeginDestroy();
	}
}
void UPassiveItemWidget::UpdateItemSetting(FTimerHandle TimerHandler, UMaterialInterface* Material, float Cost) 
{
	// Set Default Setting (Material, Handler, Cost..)
	ItemMaterial = UMaterialInstanceDynamic::Create(Material, this);
	ItemCost = Cost;
	ItemTimerHandler = TimerHandler;

	// Set Widget Value
	Timer->SetText(FText::AsNumber(Cost));
	ItemMaterial->SetScalarParameterValue(PercentageParameterName, 1.f);
	Icon->SetBrushFromMaterial(ItemMaterial);
}