// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainWidget.h"
#include "Widget/PassiveItemWidget.h"
#include "Widget/ActiveItemWidget.h"
#include "Widget/StatWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Interface/ItemDataHandler.h"

void UMainWidget::NativeConstruct()
{
    ensure(PassiveItemWidgetClass);
    ensure(ActiveItemWidgetClass);
    ensure(StatWidgetClass);

    /** Create Active Widget */
    ActiveItemWidget = CreateWidget<UActiveItemWidget>(this, ActiveItemWidgetClass);
    if (ActiveItemWidget)
    {
        // Set Widet Init & Postion
        ActiveItemWidget->UpdateItem();
        ActiveItemWidgetHorizontalBox->AddChildToHorizontalBox(ActiveItemWidget);

        // Bind Delegate : Visible Active Item
        OnChangeVisibleItemWidget.BindUObject(ActiveItemWidget, &UActiveItemWidget::ChangeVisibleItemInfo);
    }
}
void UMainWidget::UpdateItemUI(UDataAsset* CurrentItem, const bool& IsPassvieItem) 
{
    if (IsPassvieItem)
    {
        UpdatePassiveItemUI(CurrentItem);
    }
    else
    {
        UpdateActiveItemUI(CurrentItem);
    }
}
void UMainWidget::UpdatePassiveItemUI(UDataAsset* CurrentItem)
{
    TScriptInterface<class IItemDataHandler> CurItemAction = CurrentItem;
    if (CurItemAction.GetInterface())
    {
        ensure(PassiveItemWidgetClass);
        const FName ItemName = FName(CurItemAction->GetIdentifierString());

        // Remove Before Widget
        if (PassiveItemWidgetMap.Contains(ItemName))
        {
            TWeakObjectPtr<UPassiveItemWidget> PreItemWidget = PassiveItemWidgetMap.FindRef(ItemName);
            if (PreItemWidget.IsValid()) {
                PreItemWidget->RemoveFromParent();
                PassiveItemWidgetMap.Remove(ItemName);
            }
        }

        // Create Passive Item Widget..
        UPassiveItemWidget* ItemWidget = CreateWidget<UPassiveItemWidget>(this, PassiveItemWidgetClass);
        if (ItemWidget)
        {
            // Set Widet Init & Postion
            ItemWidget->UpdateItemSetting(CurItemAction->GetTimerHandler(), CurItemAction->GetItemMaterialInterface(), CurItemAction->GetDurationTime());
            PassiveItemWidgetHorizontalBox->AddChildToHorizontalBox(ItemWidget);

            // Save Array
            PassiveItemWidgetMap.Add(ItemName, ItemWidget);
        }
    }    
}
void UMainWidget::UpdateActiveItemUI(UDataAsset* CurrentItem)
{
    ActiveItemWidget->UpdateItem(CurrentItem);
}
void UMainWidget::UpdateStatUI(const FString& StatName, UMaterialInterface* Material)
{
    // Remove Before Stat Widget
    if (StatWidgetMap.Contains(StatName))
    {
        TWeakObjectPtr<UStatWidget> PreStatWidget = StatWidgetMap.FindRef(StatName);
        if (PreStatWidget.IsValid()) {
            PreStatWidget->RemoveFromParent();
            StatWidgetMap.Remove(StatName);
        }
    }

    // Create Stat Widget..
    if (Material != nullptr)
    {
        UStatWidget* StatWidget = CreateWidget<UStatWidget>(this, StatWidgetClass);
        if (StatWidget)
        {
            // Set Widet Init & Postion
            StatWidget->SetStatWidget(Material);
            StatWidgetHorizontalBox->AddChildToHorizontalBox(StatWidget);

            // Save Array
            StatWidgetMap.Add(StatName, StatWidget);
        }
    }
}
 