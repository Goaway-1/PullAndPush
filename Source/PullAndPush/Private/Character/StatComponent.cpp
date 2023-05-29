#include "Character/StatComponent.h"
#include "Net/UnrealNetwork.h"

UStatComponent::UStatComponent()
	:
	CurrentMoveSpeed(0), RocketPunchSpeed(1.f), RocketPunchRange(1.f), RocketPunchScale(1.f),
	StatFlags(0)
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
void UStatComponent::EnableStatFlag(ECharacterStat InFlag, float ChangeDuration)
{
	if(!IsStatFlagSet(InFlag))
	{
		StatFlags |= (uint8)InFlag;
		 
		// @TODO : Widget
	}
	else 
	{
		RemoveFlagTimer(InFlag);
	}

	CreateFlagTimer(InFlag, ChangeDuration);
}
void UStatComponent::DisableStatFlag(ECharacterStat InFlag)
{
	if (IsStatFlagSet(InFlag))
	{
		StatFlags &= ~(uint8)InFlag;

		RemoveFlagTimer(InFlag);

		// @TODO : Widget
	}
}
bool UStatComponent::IsStatFlagSet(ECharacterStat InFlag)
{
	return ((StatFlags & (uint8)InFlag) == (uint8)InFlag) ? true : false;
}
void UStatComponent::CreateFlagTimer(ECharacterStat InFlag, float ChangeDuration)
{
	FName FlagName = UEnum::GetValueAsName(InFlag);
	FTimerHandle Handle;
	FTimerDelegate CallbackDelegate;
	CallbackDelegate.BindLambda([=]() {
		DisableStatFlag(InFlag);
		});
	GetWorld()->GetTimerManager().SetTimer(Handle, CallbackDelegate, ChangeDuration, false);

	FlagHandles.Add(FlagName, Handle);
}
void UStatComponent::RemoveFlagTimer(ECharacterStat InFlag)
{
	FName FlagName = UEnum::GetValueAsName(InFlag);
	FTimerHandle* Handle = FlagHandles.Find(FlagName);

	if (Handle->IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(*Handle);
		FlagHandles.Remove(FlagName);
	}
}