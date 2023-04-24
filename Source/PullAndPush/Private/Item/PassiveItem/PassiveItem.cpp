#include "Item/PassiveItem/PassiveItem.h"

void UPassiveItem::UseItem(APlayableCharacter* TargetCharacter)
{
	Super::UseItem(TargetCharacter);

	// Add Timer
	const FString ItemOwner = TargetCharacter->GetName();
	AItemTimerManager::GetInstance()->AddTimer(ItemOwner, Name, DurationTime, false, this);
}

void UPassiveItem::EndActiveItem()
{
	PPLOG(Log, TEXT("%s End Active Item"), *Name);

}
