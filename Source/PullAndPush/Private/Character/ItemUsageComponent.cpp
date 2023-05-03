// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ItemUsageComponent.h"
#include "Item/Item.h"
#include "Interface/ItemActionHandler.h"
#include "Character/PlayableCharacter.h"

UItemUsageComponent::UItemUsageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}
void UItemUsageComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
void UItemUsageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
void UItemUsageComponent::PickUpItem(UItem* ItemData)
{
	check(ItemData);

	// Play "Active or Passive" Action
	CurItemData = ItemData;
	if (!ItemData->CheckIsActiveItem()) {
		StartPassiveItem();
	}
}
void UItemUsageComponent::StartActiveItem()
{
	// @TODO : 액터의 생성 및 투척
	// UI : 현재 아이템에 활성화되도록 적용
	check(CurItemData);

	TScriptInterface<class IItemActionHandler> CurItemAction = CurItemData;
	if (CurItemAction.GetInterface()) 
	{
		APlayableCharacter* OwnerCharacter = Cast<APlayableCharacter>(GetOwner());
		CurItemAction->UseItem(OwnerCharacter);

		// @TODO : 액티브 UI 활성화
		//OnUpdatePassiveUI.Execute(CurItemData);
	}
}
void UItemUsageComponent::StartPassiveItem()
{
	TScriptInterface<class IItemActionHandler> CurItemAction = CurItemData;
	if (CurItemAction.GetInterface()) 
	{
		APlayableCharacter* OwnerCharacter = Cast<APlayableCharacter>(GetOwner());
		CurItemAction->UseItem(OwnerCharacter);

		// Show 'Active Widget'
		OnUpdatePassiveUI.Execute(CurItemData);
	}
}