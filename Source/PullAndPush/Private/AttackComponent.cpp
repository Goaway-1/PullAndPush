#include "AttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Charging
	ChargingTime = 0.f;
	bIsCharging = false;
	bIsChangedSpeed = false;
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
	bIsChangedSpeed = false;

	UE_LOG(LogTemp, Log, TEXT("Try Launch"));
}
void UAttackComponent::ChargingLaunch()
{
	if (bIsCharging) {
		ChargingTime += GetWorld()->GetDeltaSeconds();

		// Time to recognize what is currently being charged
		if (ChargingTime > DecideChargingTimeSec) {
			if(!bIsChangedSpeed) ChangeMovementSpeed(MinMoveSpeed, MinJumpVelocity);

			// @TODO : Move Camera (ZoomIn/Out)
		}
	}
}
void UAttackComponent::EndLaunch()
{
	bIsCharging = false;
	bIsChangedSpeed = false;
	ChangeMovementSpeed(MaxMoveSpeed, MaxJumpVelocity);

	if (ChargingTime >= CanLaunchedTime) {
		// @TODO : Launch Punch!
		UE_LOG(LogTemp, Log, TEXT("Launched Punch!"));
	}
	UE_LOG(LogTemp, Log, TEXT("EndLaunch ChargingTime : %f"), ChargingTime);
}

void UAttackComponent::ChangeMovementSpeed(const float& NewMoveSpeed, const float& NewJumpVelocity)
{	
	UE_LOG(LogTemp, Log, TEXT("[AttackComponent] ChangeCharging Delegate is called!"));

	bIsChangedSpeed = true;
	OnCharging.Execute(NewMoveSpeed,NewJumpVelocity);
}