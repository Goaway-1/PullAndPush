#include "Item/PassiveItem/PassiveItemSpeedUp.h"

void UPassiveItemSpeedUp::UseItem(AActor* TargetActor)
{
    Super::UseItem(TargetActor);

    // Try Character Movement Speed Up
    if (!bIsItemActivated) {
        CharacterPropertyHandler->SetMovementSpeed(false, WeightValue);
    }
}

void UPassiveItemSpeedUp::EndActiveItem()
{
    Super::EndActiveItem();

    // Try Character Movement Speed Down
    CharacterPropertyHandler->SetMovementSpeed(false, -WeightValue);
}
