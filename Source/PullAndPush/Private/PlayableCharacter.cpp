#include "PlayableCharacter.h"
#include "AttackComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Curves/CurveFloat.h"

APlayableCharacter::APlayableCharacter()
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
}
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayableCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayableCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayableCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APlayableCharacter::Turn);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this, &APlayableCharacter::Jump);
	PlayerInputComponent->BindAction("Launch", IE_Pressed, this, &APlayableCharacter::TryLaunch);
	PlayerInputComponent->BindAction("Launch", IE_Released, this, &APlayableCharacter::EndLaunch);
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
void APlayableCharacter::TryLaunch()
{
	AttackComp->TryLaunch();
}
void APlayableCharacter::EndLaunch()
{
	AttackComp->EndLaunch();
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
		ZoomTimeline->SetPlayRate(3.f);
	}
	SetPlayerAttackCondition(NewCondition);
}
void APlayableCharacter::UpdateSpringArmLength(const float NewArmLength)
{
	SpringArmComp->TargetArmLength = NewArmLength;
}