#include "Widget/ActiveItemWidget.h"
#include "Components/Image.h"

void UActiveItemWidget::NativeConstruct()
{
	ensure(DefaultMaterial);
}

void UActiveItemWidget::UpdateItemMaterial(UMaterialInterface* Material) {

	if (Material) 
	{
		Icon->SetBrushFromMaterial(Material);
	}
	else
	{
		Icon->SetBrushFromMaterial(DefaultMaterial);
	}
}