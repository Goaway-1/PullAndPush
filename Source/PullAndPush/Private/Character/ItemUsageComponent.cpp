// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ItemUsageComponent.h"
#include "Item/Item.h"
#include "Interface/ItemActionHandler.h"
#include "Interface/DeployableItemHandler.h"

UItemUsageComponent::UItemUsageComponent()
	: 
	bIsReadyToThrow(0)
{
	PrimaryComponentTick.bCanEverTick = false;

}
void UItemUsageComponent::PickUpItem(UItem* ItemData)
{
	check(ItemData);

	// Play "Active or Passive" Action
	if (!ItemData->CheckIsActiveItem()) {
		TryToUsePassiveItem(ItemData);
	}
	else {
		CurActiveItemData = ItemData;
	}
}
void UItemUsageComponent::ThrowDeployableItem()
{
	if (!CurDeployableItem) return;

	PPLOG(Log,TEXT("Throw Item"));
	
	// Detach
	CurDeployableItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// Set Deployable Item Physics and Collision
	TScriptInterface<class IDeployableItemHandler> CurItemHandler = CurDeployableItem;
	if (CurItemHandler.GetInterface())
	{	
		CurItemHandler->SetActivePhysicsAndCollision();
	}

	// @TODO : 던지기

	CurDeployableItem = nullptr;
	bIsReadyToThrow = false;
}
void UItemUsageComponent::TryToUseActiveItem()
{
	if(!CurActiveItemData) return;

	PPLOG(Log, TEXT("UseActionItem"));
	TScriptInterface<class IItemActionHandler> CurItemAction = CurActiveItemData;
	if (CurItemAction.GetInterface())
	{
		// Spawn
		TSubclassOf<class AActor> DeployableItemClass = CurItemAction->GetSpawnItemClass();
		CurDeployableItem = GetWorld()->SpawnActor(DeployableItemClass);
		CurDeployableItem->SetActorLocation(GetOwner()->GetActorLocation());

		// Attach
		CurDeployableItem->AttachToActor(GetOwner(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemSocketName);

		bIsReadyToThrow = true;
		CurActiveItemData = nullptr;

		// @TODO : 액티브 UI 활성화
	}
}
void UItemUsageComponent::TryToUsePassiveItem(UItem* ItemData)
{
	TScriptInterface<class IItemActionHandler> CurItemAction = ItemData;
	if (CurItemAction.GetInterface()) 
	{
		CurItemAction->UseItem(GetOwner());

		// Show 'Active Widget'
		OnUpdatePassiveUI.Execute(ItemData);
	}
}