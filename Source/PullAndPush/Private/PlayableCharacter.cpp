#include "PlayableCharacter.h"
#include "AttackComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

APlayableCharacter::APlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneeComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneeComp"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	ChargingSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("ChargingSpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComp"));

	// Camera Setting
	SceneeComp->SetupAttachment(GetCapsuleComponent());
	InitSpringArm(SpringArmComp, 450.f, FVector(0.f, 0.f, 60.f));
	InitSpringArm(ChargingSpringArmComp, 100.f, FVector(0.f, 40.f, 70.f));
	CameraComp->SetupAttachment(SpringArmComp);
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,700.f,0.f);

	// Change Some Values if AttackComp is Charging
	AttackComp->OnCharging.BindUObject(this, &APlayableCharacter::SetMovementSpeed);
}
void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

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
void APlayableCharacter::ChargingLaunch()
{
	AttackComp->ChargingLaunch();
}
void APlayableCharacter::EndLaunch()
{
	AttackComp->EndLaunch();
}
void APlayableCharacter::SetMovementSpeed(const float& NewMoveSpeed, const float& NewJumpVelocity, const float& CameraMoveSpeed)
{
	USpringArmComponent* Arm;
	EPlayerAttackCondition NewPlayerAttackCondition;

	// Check is Zoom In or Out?
	if(GetCharacterMovement()->MaxWalkSpeed > NewMoveSpeed) 
	{
		Arm = ChargingSpringArmComp;
		NewPlayerAttackCondition = EPlayerAttackCondition::EPAC_Charging;
	}
	else
	{
		Arm = SpringArmComp;	
		NewPlayerAttackCondition = EPlayerAttackCondition::EPAC_Idle;
	}

	SetPlayerAttackCondition(NewPlayerAttackCondition);
	ZoomInOut(Arm, CameraMoveSpeed);
	GetCharacterMovement()->MaxWalkSpeed = NewMoveSpeed;
	GetCharacterMovement()->JumpZVelocity = NewJumpVelocity;
}
void APlayableCharacter::InitSpringArm(USpringArmComponent* SpringArm, const float& NewTargetArmLength, const FVector& NewSocketOffset)
{
	SpringArm->SetupAttachment(SceneeComp);
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
void APlayableCharacter::ZoomInOut(USpringArmComponent* NewSpringArm, const float& CameraMoveSpeed) 
{
	CameraComp->AttachToComponent(NewSpringArm, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(CameraComp, FVector(0.f), FRotator(0.f), false, false, CameraMoveSpeed, true, EMoveComponentAction::Type::Move, LatentInfo);
}