#include "Item/PassiveItem/PassiveItem.h"
#include "Item/ItemTimerManager.h"

void UPassiveItem::UseItem(AActor* TargetActor)
{
	Super::UseItem(TargetActor);
	ensure(TargetActor);

	// Add Timer
	const FString ItemOwner = TargetActor->GetName();
	CharacterPropertyHandler = TargetActor;
	
	// Get Timer Handler & Is Item Already Activated 
	bool bItemAlreadyActivated;
	ItemTimerHandler = AItemTimerManager::GetInstance()->AddTimer(ItemOwner, Name, DurationTime, false, this, bItemAlreadyActivated);

	bIsItemActivated = bItemAlreadyActivated;
	if (CharacterPropertyHandler.GetInterface() && !bIsItemActivated) {
		PPLOG(Log, TEXT("[%s] Item Activated %f'sec"), *Name, DurationTime);
	}
}
void UPassiveItem::EndActiveItem()
{
	PPLOG(Log, TEXT("[%s] Item Deactivated"), *Name);
}