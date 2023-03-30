#include "AttackComponent.h"
#include "RocketPunch.h"
#include "RPMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UAttackComponent::UAttackComponent() 
	:
	ChargingTime(0.f), bIsCharging(false), bIsChangeValue(false),
	RocketPunch(nullptr), bIsCanLaunch(true) 
{
	PrimaryComponentTick.bCanEverTick = true;

}
void UAttackComponent::BeginPlay(){
	Super::BeginPlay();

	RocketPunch = GetWorld()->SpawnActor<ARocketPunch>(RocketPunchClass);
	RocketPunch->SetActorLocation(GetOwner()->GetActorLocation());
	RocketPunch->SetOwner(GetOwner());
	RocketPunch->RPMovementComponent->OnReturn.BindUObject(this, &UAttackComponent::SetCanLaunch);
}
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ChargingLaunch();
}
void UAttackComponent::TryLaunch()
{
	if (!bIsCanLaunch) return;

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
			ChangeMovementSpeed(MinMoveSpeed, MinJumpVelocity);
			bIsCanLaunch = false;
		}
	}
}
void UAttackComponent::EndLaunch()
{
	if (!bIsCharging) return;

	bIsCharging = false;
	bIsChangeValue = false;
	ChangeMovementSpeed(MaxMoveSpeed, MaxJumpVelocity);
	
	// Clamp ChargingTime and Check is can launch
	ChargingTime = FMath::Clamp(ChargingTime, MinChargingTime, MaxChargingTime);
	UE_LOG(LogTemp, Log, TEXT("EndLaunch ChargingTime : %f"), ChargingTime);
	if (ChargingTime >= CanLaunchedTime) {
		check(RocketPunch);
		RocketPunch->ReadyToLaunch(ChargingTime);
	}
	else bIsCanLaunch = true;
}
void UAttackComponent::ChangeMovementSpeed(const float& NewMoveSpeed, const float& NewJumpVelocity)
{	
	UE_LOG(LogTemp, Log, TEXT("[AttackComponent] ChangeCharging Delegate is called! Excute!!"));

	bIsChangeValue = true;
	OnCharging.Execute(NewMoveSpeed,NewJumpVelocity);
}
void UAttackComponent::SetCanLaunch(const bool& Val)
{
	UE_LOG(LogTemp, Log, TEXT("[UAttackComponent] Make it possible to attack again"));

	bIsCanLaunch = Val;
}
