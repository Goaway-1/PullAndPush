// Fill out your copyright notice in the Description page of Project Settings.

// 야 참조가 너무 많은데..?

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
	if (ItemData->CheckIsActiveItem()) {
		// @TODO : 키 입력 시 실행되도록
		//StartActiveItem();
	}
	else {
		StartPassiveItem();
	}
}
void UItemUsageComponent::StartActiveItem()
{
	// @TODO : 액터의 생성 및 투척
	check(CurItemData);

	TScriptInterface<class IItemActionHandler> CurItemAction = CurItemData;
	if (CurItemAction.GetInterface()) {
		APlayableCharacter* OwnerCharacter = Cast<APlayableCharacter>(GetOwner());
		CurItemAction->UseItem(OwnerCharacter);
	}
}
void UItemUsageComponent::StartPassiveItem()
{
	// @TODO : 전략패턴 : 상황에 맞는 이벤트 발생하기
	TScriptInterface<class IItemActionHandler> CurItemAction = CurItemData;
	if (CurItemAction.GetInterface()) {
		APlayableCharacter* OwnerCharacter = Cast<APlayableCharacter>(GetOwner());
		CurItemAction->UseItem(OwnerCharacter);
	}
}