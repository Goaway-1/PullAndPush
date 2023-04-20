// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ItemUsageComponent.h"
#include "Item/Item.h"
#include "Item/ActiveItem.h"
#include "Item/PassiveItem.h"
#include "Item/ItemTimerManager.h"

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
	if (ItemData->CheckIsActiveItem()) {
		StartActiveItem();
	}
	else {
		StartPassiveItem();
	}
}
void UItemUsageComponent::StartActiveItem()
{
	// @TODO : 액터의 생성 및 투척
	switch (CurItemData->ItemActionType)
	{
	case EItemActionType::EIAT_A_Bomb:
		UE_LOG(LogTemp, Warning, TEXT("[UItemUsageComponent] Use Bomb "));
		break;
	case EItemActionType::EIAT_A_Trap:
		UE_LOG(LogTemp, Warning, TEXT("[UItemUsageComponent] Use Trap "));
		break;
	default:
		break;
	}
}
void UItemUsageComponent::StartPassiveItem()
{
	// Set Active Item effect with Timer
	UPassiveItem* CurPassiveItem = Cast<UPassiveItem>(CurItemData);
	check(CurPassiveItem);
	
	// @TODO : 상황에 맞는 이벤트 발생하기
	switch (CurPassiveItem->ItemActionType)
	{
	case EItemActionType::EIAT_P_PowerUp:
		PPLOG(Log, TEXT("Power Up"));
		break;
	case EItemActionType::EIAT_P_SpeedUp:
		PPLOG(Log, TEXT("Speed Up"));
		break;
	default:
		break;
	}

	// Add Timer
	const FString ItemOwner = GetOwner()->GetName();
	const FString ItemName = CurPassiveItem->Name;
	const EItemActionType ItemType = CurPassiveItem->ItemActionType;
	const float ItemDurationTime = CurPassiveItem->DurationTime;

	AItemTimerManager::GetInstance()->AddTimer(ItemOwner, ItemName, ItemType, ItemDurationTime, false, this);
}
void UItemUsageComponent::EndActiveItem(const EItemActionType ItemType)
{
	// @TODO : Roll Back States..
	switch (ItemType)
	{
	case EItemActionType::EIAT_P_PowerUp:
		PPLOG(Log, TEXT("Power Down"));
		break;
	case EItemActionType::EIAT_P_SpeedUp:
		PPLOG(Log, TEXT("Speed Down"));
		break;
	default:
		break;
	}

}