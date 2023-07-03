#include "Character/AttackComponent.h"
#include "RocketPunch/RocketPunch.h"
#include "RocketPunch/ClientRocketPunch.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Math/Quat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/CharacterStatHandler.h"
#include "Net/UnrealNetwork.h"

UAttackComponent::UAttackComponent() 
	:
	ChargingTime(0.f), bIsCharging(false), bIsChangeValue(false),
	bIsCanLaunch(true), RocketPunch(nullptr), ClientRocketPunch(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}
void UAttackComponent::BeginPlay(){
	Super::BeginPlay();

	// Setting CharacterSocket
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	RocketPunchSocket = OwnerCharacter.Get()->GetMesh()->GetSocketByName(RocketPunchSocketName);
	ensure(RocketPunchSocket);
}
void UAttackComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Spawn RocketPunch
	SpawnRocketPunch();
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
	if (RocketPunch && RocketPunchSocket && ChargingTime >= CanLaunchedTime) 
	{
		// Location & Rotator & Charging Percent
		const float ChargingAlpha = (ChargingTime - CanLaunchedTime) / (MaxChargingTime - CanLaunchedTime);
		const FVector LaunchLocation = RocketPunchSocket->GetSocketLocation(OwnerCharacter.Get()->GetMesh());
		const FRotator LaunchRotation = OwnerCharacter.Get()->GetControlRotation(); 
		
		// Try To Launch
		TScriptInterface<class ICharacterStatHandler> OwnerHandler = OwnerCharacter.Get();
		if (OwnerHandler.GetInterface())
		{
			const float RPAlphaSpeed = OwnerHandler->GetRocketPunchSpeed();
			const float RPAlphaRange = OwnerHandler->GetRocketPunchRange();
			const float RPAlphaSize = OwnerHandler->GetRocketPunchScale();
			
			// @TEST : 클라인 경우.. 서버는 삭제..!
			if (ClientRocketPunch)	
			{
				ClientRocketPunch->ReadyToLaunch(ChargingAlpha, GetOwner(), bIsPush, false,LaunchLocation, LaunchRotation, RPAlphaSpeed, RPAlphaRange, RPAlphaSize);
			}
			RocketPunch->ReadyToLaunch(ChargingAlpha, GetOwner(), bIsPush, false, LaunchLocation, LaunchRotation, RPAlphaSpeed, RPAlphaRange, RPAlphaSize);
		}
	}
	else bIsCanLaunch = true;
}
void UAttackComponent::SpawnRocketPunch()
{
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		ServerSpawnRocketPunch();

		if (ClientRocketPunchClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = GetOwner()->GetInstigator();

			ClientRocketPunch = GetWorld()->SpawnActor<AClientRocketPunch>(ClientRocketPunchClass, SpawnParams);
			ensure(ClientRocketPunch != nullptr);
			ClientRocketPunch->SetActorLocation(GetOwner()->GetActorLocation());
		}
	}
	else if (!RocketPunch && RocketPunchClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetOwner()->GetInstigator();

		RocketPunch = GetWorld()->SpawnActor<ARocketPunch>(RocketPunchClass, SpawnParams);
		ensure(RocketPunch != nullptr);
		RocketPunch->SetActorLocation(GetOwner()->GetActorLocation());
		RocketPunch->OutOfUse.BindUObject(this, &UAttackComponent::ClientSetCanLaunch);
		RocketPunch->OnRocketPunchCallBack.BindUObject(this, &UAttackComponent::ClientSetClientRP);
	}
}
void UAttackComponent::ServerSpawnRocketPunch_Implementation()
{
	SpawnRocketPunch();
}
void UAttackComponent::ChangeMovementSpeed(const bool& IsCharging)
{	
	bIsChangeValue = true;
	
	// Set PlayerCharacter Properties...
	TScriptInterface<class ICharacterStatHandler> CharacterPropertyHandler = GetOwner();
	if (CharacterPropertyHandler.GetInterface())
	{
		// Set Character AttackCondition & MovementSpeed
		CharacterPropertyHandler->SetPlayerAttackCondition(IsCharging);
	}
}
void UAttackComponent::SetRPForceReturn_Implementation()
{
	if (ClientRocketPunch)
	{
		ClientRocketPunch->SetForceReturn();
	}
}
void UAttackComponent::ClientSetCanLaunch_Implementation(const bool Val)
{
	UE_LOG(LogTemp, Log, TEXT("[UAttackComponent] Make it possible to attack again"));
	bIsCanLaunch = Val;
}
void UAttackComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttackComponent, RocketPunch);
}