#include "Widget/StatWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void UStatWidget::SetStatWidget(UMaterialInterface* Material)
{
	// Set Stat Material
	StatMaterial = UMaterialInstanceDynamic::Create(Material, this);
	if (StatMaterial)
	{
		StatIcon->SetBrushFromMaterial(StatMaterial);
	}
}