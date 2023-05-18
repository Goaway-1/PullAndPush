#include "Item/ItemData/PassiveItemData/PassiveItemData.h"
#include "Item/ItemTimerManager.h"

void UPassiveItemData::UseItem(AActor* TargetActor)
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
void UPassiveItemData::EndActiveItem()
{
	PPLOG(Log, TEXT("[%s] Item Deactivated"), *Name);
}