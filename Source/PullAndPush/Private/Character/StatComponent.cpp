#include "Character/StatComponent.h"
#include "GameData/CharacterStatMaterialTable.h"
#include "Interface/CharacterStatHandler.h"

UStatComponent::UStatComponent()
	:
	CurrentMoveSpeed(0), RocketPunchSpeed(1.f), RocketPunchRange(1.f), RocketPunchScale(1.f),
	StatFlags(0)
{
	PrimaryComponentTick.bCanEverTick = true;
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
		float Speed = PendingMoveSpeed + NewMoveSpeed;
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
		
		// If the stat associated with the speed has changed
		if (InFlag == ECharacterStat::Slow)
		{
			OnMoveSpeedChanged.ExecuteIfBound();
		}
	}
	else 
	{
		RemoveStatFlagTimer(InFlag);
	}

	CreateStatFlagTimer(InFlag, ChangeDuration);
}
void UStatComponent::DisableStatFlag(ECharacterStat InFlag)
{
	if (IsStatFlagSet(InFlag))
	{
		StatFlags &= ~(uint8)InFlag;
		RemoveStatFlagTimer(InFlag);

		// If the stat associated with the speed has changed
		if (InFlag == ECharacterStat::Slow)
		{
			OnMoveSpeedChanged.ExecuteIfBound();
		}
	}
}
bool UStatComponent::IsStatFlagSet(ECharacterStat InFlag)
{
	return StatFlags & (uint8)InFlag;
}
void UStatComponent::CreateStatFlagTimer(ECharacterStat InFlag, float ChangeDuration)
{
	FString FlagName = UEnum::GetValueAsString(InFlag);
	FTimerHandle Handle;
	FTimerDelegate CallbackDelegate;
	CallbackDelegate.BindLambda([=]() {
		DisableStatFlag(InFlag);
		});
	GetWorld()->GetTimerManager().SetTimer(Handle, CallbackDelegate, ChangeDuration, false);

	StatFlagHandles.Add(FlagName, Handle);

	UpdateStatWidget(FlagName, true);
}
void UStatComponent::RemoveStatFlagTimer(ECharacterStat InFlag)
{
	FString FlagName = UEnum::GetValueAsString(InFlag);
	FTimerHandle* Handle = StatFlagHandles.Find(FlagName);

	if (Handle->IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(*Handle);
		StatFlagHandles.Remove(FlagName);
	}

	UpdateStatWidget(FlagName, false);
}
void UStatComponent::UpdateStatWidget(FString StatName, bool IsCreateWidget)
{
	if (IsCreateWidget)
	{
		UMaterialInterface* StatMaterial = GetMaterialForCharacterStat(StatName);
		if (StatMaterial)
		{
			OnUpdateStatWidget.ExecuteIfBound(StatName, StatMaterial);
		}
	}
	else 
	{
		OnUpdateStatWidget.ExecuteIfBound(StatName, nullptr);
	}
}
UMaterialInterface* UStatComponent::GetMaterialForCharacterStat(FString StatName)
{
	if (StatMartialTable)
	{
		FCharacterStatMaterialTable* Row = StatMartialTable->FindRow<FCharacterStatMaterialTable>(FName(StatName), TEXT(""));
		if (Row)
		{
			return Row->MaterialUI;
		}

	}
	PPLOG(Warning, TEXT("No valid MaterialUI found in StatMartialTable : %s"),*StatName);
	return nullptr; 
}
void UStatComponent::SetPassiveStat(FPassiveStat InPassiveStat)
{
	DefaultPassiveStat = DefaultPassiveStat + InPassiveStat;
}
FPassiveStat UStatComponent::GetPassiveStat()
{
	return DefaultPassiveStat;
}