#include "RPMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RocketPunch.h"

URPMovementComponent::URPMovementComponent()
	:
	bIsReturn(false), bIsLaunch(false), CurMoveSpeed(MinMoveSpeed)
{
	PrimaryComponentTick.bCanEverTick = true;

}
void URPMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	InitSetting();
}
void URPMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckMovement();
}
void URPMovementComponent::InitSetting()
{
	OwnerActor = Cast<ARocketPunch>(GetOwner());
	OwnerActor->SetActorEnableCollision(false);
	OwnerActor->SetActorHiddenInGame(true);
	OwnerActor->SetActorTickEnabled(false);
}
void URPMovementComponent::CheckMovement()
{
	if (GetIsLaunch()) {
		if (GetIsReturn()) UpdateRotation();
		Launch();
	}
}
void URPMovementComponent::ReadyToLaunch(const float& Force)
{
	OwnerActor->SetActorLocationAndRotation(GetRPOwner()->GetActorLocation(), GetRPOwner()->GetActorRotation());
	OwnerActor->SetActorEnableCollision(true);
	OwnerActor->SetActorHiddenInGame(false);
	OwnerActor->SetActorTickEnabled(true);

	StartLoc = OwnerActor->GetActorLocation();
	EndLoc = StartLoc + (OwnerActor->GetActorForwardVector() * DefaultForce * Force);
	PreDistance = (EndLoc - StartLoc).Size();

	SetIsLaunch(true);
	SetIsReturn(false);
	SetCurMoveSpeed(MinMoveSpeed);

	UE_LOG(LogTemp, Log, TEXT("Launch RocketPunch!!"));
}
void URPMovementComponent::Launch() {
	OwnerActor->SetActorLocation(OwnerActor->GetActorLocation() + (OwnerActor->GetActorForwardVector() * CurMoveSpeed));

	CurDistance = (EndLoc - OwnerActor->GetActorLocation()).Size();

	// TODO : Check Overlap...!
	if (PreDistance <= CurDistance && GetRPOwner()) {
		StartLoc = OwnerActor->GetActorLocation();
		EndLoc = GetRPOwner()->GetActorLocation();
		PreDistance = (EndLoc - OwnerActor->GetActorLocation()).Size();

		// Return or Invisible
		if (!bIsReturn) {
			SetIsReturn(true);
			OwnerActor->SetActorEnableCollision(false);
			SetCurMoveSpeed(MaxMoveSpeed);
		}
		else {
			SetIsLaunch(false);
			OwnerActor->SetActorHiddenInGame(true);
			OwnerActor->SetActorTickEnabled(false);
		}
	}
	else PreDistance = CurDistance;
}
void URPMovementComponent::UpdateRotation() 
{
	const FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(OwnerActor->GetActorLocation(), GetRPOwner()->GetActorLocation());
	OwnerActor->SetActorRotation(NewRot);
	EndLoc = GetRPOwner()->GetActorLocation();
}
const AActor* URPMovementComponent::GetRPOwner()
{
	return OwnerActor->GetOwnerActor();
}
