#include "AttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Charging
	ChargingTime = 0.f;
	bIsCharging = false;
	bIsChangeValue = false;
}
void UAttackComponent::BeginPlay(){
	Super::BeginPlay();

}
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ChargingLaunch();
}
void UAttackComponent::TryLaunch()
{
	ChargingTime = 0.f;
	bIsCharging = true;
	bIsChangeValue = false;
}
void UAttackComponent::ChargingLaunch()
{
	if (bIsCharging && MaxChargingTime >= ChargingTime) {
		ChargingTime += GetWorld()->GetDeltaSeconds();

		// Change Speed & View if Charging
		if (!bIsChangeValue && ChargingTime > DecideChargingTime) {
			ChangeMovementSpeed(MinMoveSpeed, MinJumpVelocity, MaxChargingTime);
		}
	}
}
void UAttackComponent::EndLaunch()
{
	bIsCharging = false;
	bIsChangeValue = false;
	ChangeMovementSpeed(MaxMoveSpeed, MaxJumpVelocity, MinChargingTime);
	
	// Clamp ChargingTime and Check is can launch
	ChargingTime = FMath::Clamp(ChargingTime, MinChargingTime, MaxChargingTime);
	if (ChargingTime >= CanLaunchedTime) {
		// @TODO : Launch Punch!
		UE_LOG(LogTemp, Log, TEXT("Launched Punch!"));
	}
	UE_LOG(LogTemp, Log, TEXT("EndLaunch ChargingTime : %f"), ChargingTime);
}

void UAttackComponent::ChangeMovementSpeed(const float& NewMoveSpeed, const float& NewJumpVelocity, const float& CameraMoveSpeed)
{	
	UE_LOG(LogTemp, Log, TEXT("[AttackComponent] ChangeCharging Delegate is called!"));

	bIsChangeValue = true;
	OnCharging.Execute(NewMoveSpeed,NewJumpVelocity, CameraMoveSpeed);
}