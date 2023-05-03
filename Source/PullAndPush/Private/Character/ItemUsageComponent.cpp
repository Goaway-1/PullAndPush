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
	// @TODO : ������ ���� �� ��ô
	// UI : ���� �����ۿ� Ȱ��ȭ�ǵ��� ����
	check(CurItemData);

	TScriptInterface<class IItemActionHandler> CurItemAction = CurItemData;
	if (CurItemAction.GetInterface()) 
	{
		APlayableCharacter* OwnerCharacter = Cast<APlayableCharacter>(GetOwner());
		CurItemAction->UseItem(OwnerCharacter);

		// @TODO : ��Ƽ�� UI Ȱ��ȭ
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