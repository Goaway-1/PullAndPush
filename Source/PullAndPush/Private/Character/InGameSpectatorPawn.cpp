#include "Character/InGameSpectatorPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AInGameSpectatorPawn::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
    Super::SetupPlayerInputComponent(InInputComponent); 
	InitEnhancedInput();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InInputComponent))
	{
		if (!IsValid(SpectateAction)) return;

		EnhancedInputComponent->BindAction(SpectateAction, ETriggerEvent::Completed, this, "SpectateAnotherPlayer");
	}
}
void AInGameSpectatorPawn::InitEnhancedInput()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			SubSystem->AddMappingContext(DefaultContext, 0);
	}
}
void AInGameSpectatorPawn::SpectateAnotherPlayer()
{
	if (APlayerController* PC = GetController<APlayerController>())
	{
		PC->ServerViewNextPlayer();
	}
}