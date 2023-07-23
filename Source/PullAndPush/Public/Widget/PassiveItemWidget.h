#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PassiveItemWidget.generated.h"

/**
 *	Passive Item Widget for player hud
 */
UCLASS(Abstract)
class PULLANDPUSH_API UPassiveItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateItemSetting(FTimerHandle TimerHandler, UMaterialInterface* Material, float Cost);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Timer;

private:
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> ItemMaterial;

	UPROPERTY(Transient)
	float ItemCost;

	const FName PercentageParameterName = "Percentage";
	FTimerHandle ItemTimerHandler;
};
