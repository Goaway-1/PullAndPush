#include "AttackComponent.h"
#include "RocketPunch.h"
#include "AttackWeapon.h"
#include "RPMovementComponent.h"
#include "RPCollisionComponent.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Math/Quat.h"
#include "GameFramework/CharacterMovementComponent.h"

UAttackComponent::UAttackComponent() 
	:
	ChargingTime(0.f), bIsCharging(false), bIsChangeValue(false),
	bIsCanLaunch(true), RocketPunch(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;

}
void UAttackComponent::BeginPlay(){
	Super::BeginPlay();

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
			ChangeMovementSpeed(MinMoveSpeed, MinJumpVelocity);
			bIsCanLaunch = false;
		}
	}
}
void UAttackComponent::EndLaunch(bool bIsPush)
{
	if (!bIsCharging) return;

	bIsCharging = false;
	bIsChangeValue = false;
	ChangeMovementSpeed(MaxMoveSpeed, MaxJumpVelocity);
	
	// Clamp ChargingTime and Check is can launch
	ChargingTime = FMath::Clamp(ChargingTime, MinChargingTime, MaxChargingTime);
	UE_LOG(LogTemp, Log, TEXT("EndLaunch ChargingTime : %f"), ChargingTime);
	if (RocketPunch && RocketPunchSocket && ChargingTime >= CanLaunchedTime) {
		//Location & Rotator
		const FVector LaunchLocation = RocketPunchSocket->GetSocketLocation(OwnerCharacter->GetMesh());
		const FRotator LaunchRotation = OwnerCharacter->GetControlRotation();
		
		// @TODO : RocketPunch가 아닌 다른 무기인 경우 -> CurAttackWeapon에 대입만하고 사용하면 된다.
		CurAttackWeapon = RocketPunch;
		if (CurAttackWeapon)
		{
			CurAttackWeapon->ReadyToLaunch(ChargingTime, GetOwner(), bIsPush, LaunchLocation, LaunchRotation);
		}
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