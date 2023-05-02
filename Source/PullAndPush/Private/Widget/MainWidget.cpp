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

    // Map으로 할 때, FPrimartAssetId로 하고, 타입이 이제 Item
    TScriptInterface<class IItemActionHandler> CurItemAction = CurrentItem;
    if (CurItemAction.GetInterface()) 
    {
        FName ItemName = FName(CurItemAction->GetIdentifierString());
        if (ItemMap.Contains(ItemName)) 
        {
            // Remove Before Widget
            UItemWidget* CItemWidget = ItemMap.FindRef(ItemName);
            if (CItemWidget) {
                UE_LOG(LogTemp, Warning, TEXT("Remove UI!"));

                CItemWidget->RemoveFromParent();
                ItemMap.Remove(ItemName);
                CItemWidget = nullptr;
            }
        }

        // Create Item Widget..
        UItemWidget* ItemWidget = CreateWidget<UItemWidget>(this, ItemWidgetClass);
        if (ItemWidget)
        {
            // Set Widget
            ItemWidget->SetInitItem(CurItemAction->GetTimerHandler(), CurItemAction->GetItemMaterialInterface(), CurItemAction->GetDurationTime());
            ItemHorizontalBox->AddChildToHorizontalBox(ItemWidget);

            // Save Array
            ItemMap.Add(ItemName, ItemWidget);
            UE_LOG(LogTemp, Warning, TEXT("Generagte UI"));
        }
    }
}