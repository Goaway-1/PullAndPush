// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ItemUsageComponent.h"
#include "Item/ItemData/ItemData.h"
#include "Interface/ItemActionHandler.h"
#include "Interface/DeployableItemHandler.h"

UItemUsageComponent::UItemUsageComponent()
	: 
	bIsReadyToThrow(0)
{
	PrimaryComponentTick.bCanEverTick = false;

}
void UItemUsageComponent::PickUpItem(UItemData* ItemData)
{
	check(ItemData);

	// Play "Active or Passive" Action
	if (!ItemData->CheckIsActiveItem()) {
		TryToUsePassiveItem(ItemData);
	}
	else {
		CurActiveItemData = ItemData;

		// Show 'Passive Widget'
		OnItemWidgetUpdate.Execute(CurActiveItemData, false);
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

	// @TODO : ´øÁö±â

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

		// Hide 'Passive Widget'
		OnItemWidgetUpdate.Execute(CurActiveItemData, false);
	}
}
void UItemUsageComponent::TryToUsePassiveItem(UItemData* ItemData)
{
	TScriptInterface<class IItemActionHandler> CurItemAction = ItemData;
	if (CurItemAction.GetInterface()) 
	{
		CurItemAction->UseItem(GetOwner());

		// Show 'Active Widget'
		OnItemWidgetUpdate.Execute(ItemData, true);
	}
}