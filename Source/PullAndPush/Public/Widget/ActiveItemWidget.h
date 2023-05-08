// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActiveItemWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PULLANDPUSH_API UActiveItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/**
	* Update Active item Material
	* @param Material		Set Default Material if Material is nullptr
	*/
	void UpdateItemMaterial(UMaterialInterface* Material = nullptr);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Icon;

	UPROPERTY(EditDefaultsOnly, Category="Material")
	TObjectPtr<class UMaterialInterface> DefaultMaterial;
};
