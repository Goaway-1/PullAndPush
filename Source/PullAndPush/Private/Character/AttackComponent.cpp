#include "Character/AttackComponent.h"
#include "RocketPunch/RocketPunch.h"
#include "Interface/AttackWeapon.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Math/Quat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/CharacterPropertyHandler.h"

UAttackComponent::UAttackComponent() 
	:
	ChargingTime(0.f), bIsCharging(false), bIsChangeValue(false),
	bIsCanLaunch(true), RocketPunch(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;

}
void UAttackComponent::BeginPlay(){
	Super::BeginPlay();

	// @TODO : instigator :: GetWorld()->SpawnActorDeferred<ARocketPunch>()
	RocketPunch = GetWorld()->SpawnActor<ARocketPunch>(RocketPunchClass);
	RocketPunch->SetActorLocation(GetOwner()->GetActorLocation());
	RocketPunch->OutOfUse.BindUObject(this, &UAttackComponent::SetCanLaunch);	

	// Get Socket
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	RocketPunchSocket = OwnerCharacter->GetMesh()->GetSocketByName("RocketPunch");
	check(RocketPunchSocket);
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
	UE_LOG(LogTemp, Log, TEXT("EndLaunch ChargingTime : %f"), ChargingTime);
	if (RocketPunch && RocketPunchSocket && ChargingTime >= CanLaunchedTime) {
		// Location & Rotator & Charging Percent
		const FVector LaunchLocation = RocketPunchSocket->GetSocketLocation(OwnerCharacter->GetMesh());
		const FRotator LaunchRotation = OwnerCharacter->GetControlRotation();
		const float ChargingAlpha = (ChargingTime - CanLaunchedTime) / (MaxChargingTime - CanLaunchedTime);
		
		// Set ReadyToLaunch
		TScriptInterface<class IAttackWeapon> CurAttackWeapon = RocketPunch;
		if (CurAttackWeapon.GetInterface()) {
			CurAttackWeapon->ReadyToLaunch(ChargingAlpha, GetOwner(), bIsPush, LaunchLocation, LaunchRotation);
		}
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
		if(IsCharging) CharacterPropertyHandler->SetPlayerAttackCondition(EPlayerAttackCondition::EPAC_Charging);
		else CharacterPropertyHandler->SetPlayerAttackCondition(EPlayerAttackCondition::EPAC_Idle);

		CharacterPropertyHandler->SetMovementSpeed(IsCharging);
	}
}
void UAttackComponent::SetCanLaunch(const bool& Val)
{
	UE_LOG(LogTemp, Log, TEXT("[UAttackComponent] Make it possible to attack again"));
	bIsCanLaunch = Val;
}