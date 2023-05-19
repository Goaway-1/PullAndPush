#include "Item/ItemData/PassiveItemData/PassiveItemDataSpeedUp.h"

void UPassiveItemDataSpeedUp::UseItem(AActor* TargetActor)
{
    Super::UseItem(TargetActor);

    // Try Character Movement Speed Up
    if (!bIsItemActivated) {
        CharacterPropertyHandler->SetMovementSpeed(WeightValue);
    }
}

void UPassiveItemDataSpeedUp::EndActiveItem()
{
    Super::EndActiveItem();

    // Try Character Movement Speed Down
    CharacterPropertyHandler->SetMovementSpeed(-WeightValue);
}
