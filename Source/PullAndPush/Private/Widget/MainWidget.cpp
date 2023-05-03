// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainWidget.h"
#include "Widget/ItemWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Interface/ItemActionHandler.h"

UMainWidget::UMainWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    
}
void UMainWidget::UpdateItemUI(UDataAsset* CurrentItem) {
    ensure(ItemWidgetClass);

    TScriptInterface<class IItemActionHandler> CurItemAction = CurrentItem;
    if (CurItemAction.GetInterface())
    {
        const FName ItemName = FName(CurItemAction->GetIdentifierString());

        // Remove Before Widget
        if (ItemWidgetMap.Contains(ItemName))
        {
            TWeakObjectPtr<UItemWidget> PreItemWidget = ItemWidgetMap.FindRef(ItemName);
            if (PreItemWidget.IsValid()) {
                PreItemWidget->RemoveFromParent();
                ItemWidgetMap.Remove(ItemName);
            }
        }

        // Create Item Widget..
        UItemWidget* ItemWidget = CreateWidget<UItemWidget>(this, ItemWidgetClass);
        if (ItemWidget)
        {
            // Set Widet Init & Postion
            ItemWidget->SetInitItem(CurItemAction->GetTimerHandler(), CurItemAction->GetItemMaterialInterface(), CurItemAction->GetDurationTime());
            ItemHorizontalBox->AddChildToHorizontalBox(ItemWidget);

            // Save Array
            ItemWidgetMap.Add(ItemName, ItemWidget);
        }
    }
}