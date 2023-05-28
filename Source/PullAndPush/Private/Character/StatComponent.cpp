#include "Character/StatComponent.h"
#include "Net/UnrealNetwork.h"

UStatComponent::UStatComponent()
	:
	CurrentMoveSpeed(0), RocketPunchSpeed(1.f), RocketPunchRange(1.f), RocketPunchScale(1.f)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	ModifyMoveSpeed(DefaultMoveSpeed);
}
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCurrnentMovementSpeed();
}
bool UStatComponent::ModifyMoveSpeed(const float NewMoveSpeed)
{
	if (NewMoveSpeed > 0.f || NewMoveSpeed < 0.f) {
		float Speed = CurrentMoveSpeed + NewMoveSpeed;
		PendingMoveSpeed.exchange(Speed);
		return true;
	}
	return false;
}
void UStatComponent::UpdateCurrnentMovementSpeed()
{
	// if MoveSpeed is Changed.
	if (PendingMoveSpeed != CurrentMoveSpeed) 
	{
		CurrentMoveSpeed = PendingMoveSpeed;
		OnMoveSpeedChanged.ExecuteIfBound();
	}
}