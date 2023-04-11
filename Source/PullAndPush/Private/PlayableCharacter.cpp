#include "PlayableCharacter.h"
#include "AttackComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Curves/CurveFloat.h"
#include "DrawDebugHelpers.h"

APlayableCharacter::APlayableCharacter()
	:
	bIsMoveToLocation(false), TargetLocation(FVector(0.f)), StartLocation(FVector(0.f)), MoveToLocationSpeed(5000.f), bIsMoveToActor(false), MoveTargetActor(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComp"));

	// Camera Setting
	InitSpringArm(SpringArmComp, 450.f, FVector(0.f, 0.f, 60.f));
	CameraComp->SetupAttachment(SpringArmComp);
	bUseControllerRotationYaw = false;

	// ZoomIn & Out
	ZoomTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomTimeline"));
	ZoomInterpFunction.BindUFunction(this, FName("UpdateSpringArmLength"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,700.f,0.f);

	// Change Some Values if AttackComp is Charging
	AttackComp->OnCharging.BindUObject(this, &APlayableCharacter::SetMovementSpeed);
}
void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitZoomTimeLine();
}
void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetPlayerView();

	// If Hit Event is Called.
	MoveToLocation(DeltaTime);
	MoveToActor();
}
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayableCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayableCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayableCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APlayableCharacter::Turn);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this, &APlayableCharacter::Jump);
	PlayerInputComponent->BindAction<FInputSwitchInventoryDelegate>("Push", IE_Pressed, this, &APlayableCharacter::TryLaunch,true);
	PlayerInputComponent->BindAction<FInputSwitchInventoryDelegate>("Pull", IE_Pressed, this, &APlayableCharacter::TryLaunch,false);
	PlayerInputComponent->BindAction("Push", IE_Released, this, &APlayableCharacter::EndLaunch);
	PlayerInputComponent->BindAction("Pull", IE_Released, this, &APlayableCharacter::EndLaunch);
}
void APlayableCharacter::MoveForward(float NewAxisValue) {
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, NewAxisValue);
}
void APlayableCharacter::MoveRight(float NewAxisValue) {
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, NewAxisValue);
}
void APlayableCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}
void APlayableCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}
void APlayableCharacter::TryLaunch(bool IsPush)
{
	if (AttackComp->TryLaunch()) {
		// Set Attack Type if can Charnging
		bIsPush = IsPush;
	}
}
void APlayableCharacter::EndLaunch()
{
	AttackComp->EndLaunch(bIsPush);
}
void APlayableCharacter::SetMovementSpeed(const float& NewMoveSpeed, const float& NewJumpVelocity)
{
	// Check is Zoom In or Out?
	const EPlayerAttackCondition NewPlayerAttackCondition = (GetCharacterMovement()->MaxWalkSpeed > NewMoveSpeed) ?
		EPlayerAttackCondition::EPAC_Charging : EPlayerAttackCondition::EPAC_Idle;

	ZoomInOut(NewPlayerAttackCondition);
	GetCharacterMovement()->MaxWalkSpeed = NewMoveSpeed;
	GetCharacterMovement()->JumpZVelocity = NewJumpVelocity;
}
void APlayableCharacter::InitSpringArm(USpringArmComponent* SpringArm, const float& NewTargetArmLength, const FVector& NewSocketOffset)
{
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = NewTargetArmLength;
	SpringArm->SocketOffset = NewSocketOffset;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
}
void APlayableCharacter::SetPlayerView()
{
	if (GetPlayerAttackCondition() == EPlayerAttackCondition::EPAC_Charging) {
		FRotator NewRot = GetControlRotation();
		NewRot.Roll = 0;
		NewRot.Pitch = 0;
		SetActorRotation(NewRot);
	}
}
void APlayableCharacter::InitZoomTimeLine()
{
	if (ZoomTimeline && ZoomCurve)
	{
		ZoomTimeline->AddInterpFloat(ZoomCurve, ZoomInterpFunction);
		ZoomTimeline->SetLooping(false);
	}
	else UE_LOG(LogTemp, Warning, TEXT("[PlayerableCharacter] ZoomCurve is not exist"));
}
void APlayableCharacter::ZoomInOut(const EPlayerAttackCondition NewCondition)
{
	if (NewCondition == EPlayerAttackCondition::EPAC_Charging) {
		ZoomTimeline->Play();
		ZoomTimeline->SetPlayRate(1.f);
	}
	else {
		ZoomTimeline->Reverse();
		ZoomTimeline->SetPlayRate(2.f);
	}
	SetPlayerAttackCondition(NewCondition);
}
void APlayableCharacter::UpdateSpringArmLength(const float NewArmLength)
{
	SpringArmComp->TargetArmLength = NewArmLength;
}
void APlayableCharacter::KnockBackActor(const FVector& DirVec)
{
	GetCharacterMovement()->AddImpulse(DirVec);
}
void APlayableCharacter::SetMoveToLocation(const FVector& HitVector)
{
	bIsMoveToLocation = true;
	TargetLocation = HitVector;
	StartLocation = GetActorLocation();
}
void APlayableCharacter::MoveToLocation(float DeltaTime)
{
	if (bIsMoveToLocation) {
		const FVector Direction = (TargetLocation - StartLocation).GetSafeNormal();	
		const FVector NewLocation = StartLocation + (Direction * MoveToLocationSpeed * DeltaTime);
		
		DrawDebugLine(GetWorld(), StartLocation, NewLocation, FColor::Green, false, 10.f);
		DrawDebugSphere(GetWorld(),NewLocation, 12.f, 25,FColor::Red,false, 10.f);
		StartLocation = NewLocation;
		SetActorLocation(NewLocation);

		// Stop Moving if close to TargetLocation
		if (FVector::Distance(NewLocation, TargetLocation) < StopToMoveDistance) {
			bIsMoveToLocation = false;
		}
	}
}
void APlayableCharacter::SetMoveToActor(AActor* TargetActor)
{
	if (IsValid(TargetActor)) {
		bIsMoveToActor = true;
		MoveTargetActor = TargetActor;
	}
	else {
		bIsMoveToActor = false;
		MoveTargetActor = nullptr;
	}
}
void APlayableCharacter::MoveToActor()
{
	if (bIsMoveToActor && MoveTargetActor) {
		SetActorLocation(MoveTargetActor->GetActorLocation());
	}
}