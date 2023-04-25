#include "Item/PassiveItem/PassiveItem.h"
#include "Item/ItemTimerManager.h"

void UPassiveItem::UseItem(AActor* TargetActor)
{
	Super::UseItem(TargetActor);

	TargetActorPtr = TargetActor;

	// Add Timer
	const FString ItemOwner = TargetActorPtr->GetName();
	bIsItemActivated = AItemTimerManager::GetInstance()->AddTimer(ItemOwner, Name, DurationTime, false, this);
}

void UPassiveItem::EndActiveItem()
{
	PPLOG(Log, TEXT("%s End Active Item"), *Name);

}