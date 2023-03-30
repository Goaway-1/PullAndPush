#include "AttackComponent.h"
#include "RocketPunch.h"
#include "GameFramework/CharacterMovementComponent.h"

UAttackComponent::UAttackComponent() 
	:
	ChargingTime(0.f), bIsCharging(false), bIsChangeValue(false),
	RocketPunch(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;

}
void UAttackComponent::BeginPlay(){
	Super::BeginPlay();

	// 추가적으로 부가적인 기능이 있어야 함 -> 꺼둔다던지...
	RocketPunch = GetWorld()->SpawnActor<ARocketPunch>(RocketPunchClass);
	RocketPunch->SetActorLocation(GetOwner()->GetActorLocation());
	RocketPunch->SetOwner(GetOwner());
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
			ChangeMovementSpeed(MinMoveSpeed, MinJumpVelocity);
		}
	}
}
void UAttackComponent::EndLaunch()
{
	bIsCharging = false;
	bIsChangeValue = false;
	ChangeMovementSpeed(MaxMoveSpeed, MaxJumpVelocity);
	
	// Clamp ChargingTime and Check is can launch
	ChargingTime = FMath::Clamp(ChargingTime, MinChargingTime, MaxChargingTime);
	if (ChargingTime >= CanLaunchedTime) {
		check(RocketPunch);
		RocketPunch->ReadyToLaunch(ChargingTime);
	}
	UE_LOG(LogTemp, Log, TEXT("EndLaunch ChargingTime : %f"), ChargingTime);
}

void UAttackComponent::ChangeMovementSpeed(const float& NewMoveSpeed, const float& NewJumpVelocity)
{	
	UE_LOG(LogTemp, Log, TEXT("[AttackComponent] ChangeCharging Delegate is called!"));

	bIsChangeValue = true;
	OnCharging.Execute(NewMoveSpeed,NewJumpVelocity);
}