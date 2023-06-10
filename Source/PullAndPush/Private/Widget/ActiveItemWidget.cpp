#include "Widget/ActiveItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Interface/ItemDataHandler.h"

void UActiveItemWidget::NativeConstruct()
{
	ensure(DefaultMaterial);

	ChangeVisibleItemInfo(false);
}
void UActiveItemWidget::UpdateItem(UDataAsset* ItemData)
{
	TScriptInterface<class IItemDataHandler> CurItemAction = ItemData;
	if (CurItemAction.GetInterface())
	{
		Icon->SetBrushFromMaterial(CurItemAction->GetItemMaterialInterface());
		CurrentItemData = ItemData;
	}
	else
	{
		Icon->SetBrushFromMaterial(DefaultMaterial);
		CurrentItemData = nullptr;
	}
}
void UActiveItemWidget::ChangeVisibleItemInfo(bool bVisibility)
{
	FText InNameText = FText::FText();
	FText InDescriptionText = FText::FText();

	// Check
	if (bVisibility && CurrentItemData.IsValid())
	{
		TextCanvasPanel->SetVisibility(ESlateVisibility::Visible);
				
		TScriptInterface<class IItemDataHandler> CurItemAction = CurrentItemData.Get();
		if (CurItemAction.GetInterface())
		{
			InNameText = FText::FromString(CurItemAction->GetItemName());
			InDescriptionText = FText::FromString(CurItemAction->GetItemDescription());
		}
	}
	else
	{
		TextCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
	}

	// Set Text
	NameText->SetText(InNameText);
	DescriptionText->SetText(InDescriptionText);
}