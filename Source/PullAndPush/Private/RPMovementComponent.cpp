#include "RPMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RocketPunch.h"

URPMovementComponent::URPMovementComponent()
	:
	bIsReturn(false), bIsLaunch(false), bIsForceReturn(false)
{
	PrimaryComponentTick.bCanEverTick = true;

	CurMoveSpeed = MinMoveSpeed;
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
	Owner = Cast<ARocketPunch>(GetOwner());
	Owner->SetActorEnableCollision(false);
	Owner->SetActorHiddenInGame(true);
	Owner->SetActorTickEnabled(false);
}
void URPMovementComponent::CheckMovement()
{
	if (bIsLaunch) {
		if (bIsReturn) UpdateRotation();
		UpdateLocation();
	}
}
void URPMovementComponent::Launch(const float& Force, AActor* InCasterActor, const FVector& InVec, const FRotator& InRot)
{
	if (CasterActor == nullptr) CasterActor = InCasterActor;

	// Rocket Punch Setting
	Owner->SetActorLocationAndRotation(InVec, InRot);
	Owner->SetActorEnableCollision(true);
	Owner->SetActorHiddenInGame(false);
	Owner->SetActorTickEnabled(true);
	Owner->SetCollisionSimulatePhysics(true);

	// Movement Setting
	bIsLaunch = true;
	bIsReturn = false;
	bIsForceReturn = false;
	SetCurMoveSpeed(MinMoveSpeed);

	// to Target Distance
	StartLoc = Owner->GetActorLocation();
	EndLoc = StartLoc + (Owner->GetActorForwardVector() * DefaultForce * Force);
	PreDistance = (EndLoc - StartLoc).Size();
}
void URPMovementComponent::UpdateLocation() {
	Owner->SetActorLocation(Owner->GetActorLocation() + (Owner->GetActorForwardVector() * CurMoveSpeed));

	// Check is nearby target pos
	CurDistance = (EndLoc - Owner->GetActorLocation()).Size();
	if (GetIsForceReturn() || (PreDistance <= CurDistance && CasterActor)) {
		StartLoc = Owner->GetActorLocation();
		EndLoc = CasterActor->GetActorLocation();
		PreDistance = (EndLoc - Owner->GetActorLocation()).Size();

		// Return or Invisible
		if (!GetIsReturn()) {
			bIsReturn = true;
			bIsForceReturn = false;
			Owner->SetActorEnableCollision(false);
			Owner->SetCollisionSimulatePhysics(false);
			SetCurMoveSpeed(MaxMoveSpeed);
		}
		else {
			bIsLaunch = false;
			SetCanLaunch(true);
			Owner->SetActorHiddenInGame(true);
			Owner->SetActorTickEnabled(true);
		}
	}
	else PreDistance = CurDistance;
}
void URPMovementComponent::UpdateRotation() 
{
	const FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), CasterActor->GetActorLocation());
	Owner->SetActorRotation(NewRot);
	EndLoc = CasterActor->GetActorLocation();
}
void URPMovementComponent::SetCanLaunch(const bool& Val)
{
	UE_LOG(LogTemp, Log, TEXT("[RPMovementComponent] OnReturn Delegate is called!"));
	OnReturn.Execute(Val);
}
void URPMovementComponent::SetIsForceReturn(const bool& Val)
{
	bIsForceReturn = Val;
}