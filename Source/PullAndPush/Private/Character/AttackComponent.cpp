#include "Character/AttackComponent.h"
#include "RocketPunch/RocketPunch.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Math/Quat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/CharacterPropertyHandler.h"

UAttackComponent::UAttackComponent() 
	:
	ChargingTime(0.f), bIsCharging(false), bIsChangeValue(false),
	bIsCanLaunch(true), RocketPunch(nullptr),
	RPAlphaSpeed(1.f), RPAlphaRange(1.f), RPAlphaSize(1.f)
{
	PrimaryComponentTick.bCanEverTick = true;

}
void UAttackComponent::BeginPlay(){
	Super::BeginPlay();

	RPAlphaSpeed = 1.f;
	RPAlphaRange = 1.f;
	RPAlphaSize = 1.f;

	// Spawn RocketPunch & Setting CharacterSocket
	RocketPunch = GetWorld()->SpawnActor<ARocketPunch>(RocketPunchClass);
	RocketPunch->SetActorLocation(GetOwner()->GetActorLocation());
	RocketPunch->OutOfUse.BindUObject(this, &UAttackComponent::SetCanLaunch);	

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	RocketPunchSocket = OwnerCharacter.Get()->GetMesh()->GetSocketByName(RocketPunchSocketName);
	ensure(RocketPunchSocket);
}
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ChargingLaunch();
}
bool UAttackComponent::TryLaunch()
{
	if (!bIsCanLaunch) return false;

	ChargingTime = 0.f;
	bIsCharging = true;
	bIsChangeValue = false;
	return true;
}
void UAttackComponent::ChargingLaunch()
{
	if (bIsCharging && MaxChargingTime >= ChargingTime) {
		ChargingTime += GetWorld()->GetDeltaSeconds();			

		// Change Speed & View if Charging
		if (!bIsChangeValue && ChargingTime > DecideChargingTime) {
			ChangeMovementSpeed(bIsCharging);
			bIsCanLaunch = false;
		}
	}
}
void UAttackComponent::EndLaunch(bool bIsPush)
{
	if (!bIsCharging) return;

	bIsCharging = false;
	bIsChangeValue = false;
	ChangeMovementSpeed(bIsCharging);
	
	// Clamp ChargingTime and Check is can launch
	ChargingTime = FMath::Clamp(ChargingTime, MinChargingTime, MaxChargingTime);
	PPLOG(Log, TEXT("EndLaunch ChargingTime : %f"), ChargingTime);
	if (RocketPunch && RocketPunchSocket && ChargingTime >= CanLaunchedTime) {
		// Location & Rotator & Charging Percent
		const FVector LaunchLocation = RocketPunchSocket->GetSocketLocation(OwnerCharacter.Get()->GetMesh());
		const FRotator LaunchRotation = OwnerCharacter.Get()->GetControlRotation();
		const float ChargingAlpha = (ChargingTime - CanLaunchedTime) / (MaxChargingTime - CanLaunchedTime);
		
		// Set ReadyToLaunch
		RocketPunch->ReadyToLaunch(ChargingAlpha, GetOwner(), bIsPush, LaunchLocation, LaunchRotation, RPAlphaSpeed, RPAlphaRange, RPAlphaSize);
	}
	else bIsCanLaunch = true;
}
void UAttackComponent::ChangeMovementSpeed(const bool& IsCharging)
{	
	bIsChangeValue = true;
	
	// Set PlayerCharacter Properties...
	TScriptInterface<class ICharacterPropertyHandler> CharacterPropertyHandler = GetOwner();
	if (CharacterPropertyHandler.GetInterface())
	{
		// Set Character AttackCondition & MovementSpeed
		CharacterPropertyHandler->SetPlayerAttackCondition(IsCharging);
	}
}
void UAttackComponent::SetCanLaunch(const bool& Val)
{
	UE_LOG(LogTemp, Log, TEXT("[UAttackComponent] Make it possible to attack again"));
	bIsCanLaunch = Val;
}