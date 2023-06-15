#include "Player/UIController.h"

AUIController::AUIController() 
{
	bShowMouseCursor = true;			
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}
void AUIController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
}