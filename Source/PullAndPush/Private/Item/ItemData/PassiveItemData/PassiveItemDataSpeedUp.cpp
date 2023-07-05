#include "Item/ItemData/PassiveItemData/PassiveItemDataSpeedUp.h"

void UPassiveItemDataSpeedUp::UsePassiveItem(AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated)
{
    Super::UsePassiveItem(TargetActor, Handler, InPassiveItemAlreadyActivated);

    // Try Character Movement Speed Up
    if (CharacterPropertyHandler.GetInterface()) {
        CharacterPropertyHandler->SetMovementSpeed(WeightValue);
    }
}
void UPassiveItemDataSpeedUp::EndPassiveItem()
{
    Super::EndPassiveItem();

    // Try Character Movement Speed Down
    if (CharacterPropertyHandler.GetInterface())
    {
        CharacterPropertyHandler->SetMovementSpeed(-WeightValue);
    }
}
