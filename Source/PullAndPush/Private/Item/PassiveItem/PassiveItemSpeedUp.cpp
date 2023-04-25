#include "Item/PassiveItem/PassiveItemSpeedUp.h"

void UPassiveItemSpeedUp::UseItem(AActor* TargetActor)
{
    Super::UseItem(TargetActor);

    // Try Character Movement Speed Up
    if (TargetActorPtr && !bIsItemActivated) {
        TScriptInterface<class ICharacterPropertyHandler> CharacterPropertyHandler = TargetActorPtr;
        if (CharacterPropertyHandler.GetInterface())
        {
            PPLOG(Log, TEXT("[Speed Up] Item is Activated"));
            CharacterPropertyHandler->SetMovementSpeed(false, WeightValue);
        }
    }
}

void UPassiveItemSpeedUp::EndActiveItem()
{
    Super::EndActiveItem();

    // Try Character Movement Speed Down
    if (TargetActorPtr) {
        TScriptInterface<class ICharacterPropertyHandler> CharacterPropertyHandler = TargetActorPtr;
        if (CharacterPropertyHandler.GetInterface())
        {
            PPLOG(Log, TEXT("[Speed Up] Item is Deactivated!"));
            CharacterPropertyHandler->SetMovementSpeed(false, -WeightValue);
        }
    }
}
