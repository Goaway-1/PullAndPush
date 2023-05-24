#include "Item/ItemData/PassiveItemData/PassiveItemData.h"

void UPassiveItemData::UsePassiveItem(AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated)
{
	Super::UsePassiveItem(TargetActor, Handler, InPassiveItemAlreadyActivated);
	IsValid(TargetActor);

	// Get Timer Handler & Is Item Already Activated 
	CharacterPropertyHandler = TargetActor;
	ItemTimerHandler = Handler;
	bIsItemActivated = InPassiveItemAlreadyActivated;
	if (CharacterPropertyHandler.GetInterface() && !bIsItemActivated) {
		PPLOG(Log, TEXT("[%s] Item Activated %f'sec"), *Name, DurationTime);
	}
}
void UPassiveItemData::EndPassiveItem()
{
	PPLOG(Log, TEXT("[%s] Item Deactivated"), *Name);
}