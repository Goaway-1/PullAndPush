// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainWidget.h"
#include "Widget/PassiveItemWidget.h"
#include "Widget/ActiveItemWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Interface/ItemActionHandler.h"

void UMainWidget::NativeConstruct()
{
    ensure(PassiveItemWidgetClass);
    ensure(ActiveItemWidgetClass);

    /** Create Active Widget */
    ActiveItemWidget = CreateWidget<UActiveItemWidget>(this, ActiveItemWidgetClass);
    if (ActiveItemWidget)
    {
        // Set Widet Init & Postion
        ActiveItemWidget->UpdateItem();
        ItemHorizontalBox->AddChildToHorizontalBox(ActiveItemWidget);

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
    TScriptInterface<class IItemActionHandler> CurItemAction = CurrentItem;
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
            ItemHorizontalBox->AddChildToHorizontalBox(ItemWidget);

            // Save Array
            PassiveItemWidgetMap.Add(ItemName, ItemWidget);
        }
    }    
}
void UMainWidget::UpdateActiveItemUI(UDataAsset* CurrentItem)
{
    ActiveItemWidget->UpdateItem(CurrentItem);
}
 