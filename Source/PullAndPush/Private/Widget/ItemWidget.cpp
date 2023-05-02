// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ItemWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

void UItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Set Widget Value..
	float Value = UKismetSystemLibrary::K2_GetTimerRemainingTimeHandle(GetWorld(), ItemTimerHandler);
	Timer->SetText(FText::AsNumber(Value));

	const float Parcent = Value / DefaultItemCost;
	ItemMaterial->SetScalarParameterValue("Percentage", Parcent);
	
	if (Value < KINDA_SMALL_NUMBER)
	{
		RemoveFromParent();
	}
}
void UItemWidget::SetInitItem(FTimerHandle TimerHandler, UMaterialInterface* Material, float Cost) {
	// Set Default Setting (Material, Handler, Cost..)
	ItemMaterial = UMaterialInstanceDynamic::Create(Material, this);
	DefaultItemCost = Cost;
	ItemTimerHandler = TimerHandler;

	Timer->SetText(FText::AsNumber(Cost));
	ItemMaterial->SetScalarParameterValue("Percentage", 1.f);
	ItemIcon->SetBrushFromMaterial(ItemMaterial);
}