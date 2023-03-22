#include "PlayableCharacter.h"
#include "AttackComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayableCharacter::APlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComp"));

	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->SetupAttachment(SpringArmComp);

	// Camera Setting
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->TargetArmLength = 450.f;
	SpringArmComp->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArmComp->bInheritPitch = true;
	SpringArmComp->bInheritRoll = true;
	SpringArmComp->bInheritYaw = true;
	SpringArmComp->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,700.f,0.f);

	AttackComp->OnCharging.BindUObject(this, &APlayableCharacter::SetMovementSpeed);
}
void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

}
void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayableCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayableCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayableCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APlayableCharacter::Turn);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this, &APlayableCharacter::Jump);
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
void APlayableCharacter::SetMovementSpeed(const float& NewMoveSpeed, const float& NewJumpVelocity)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMoveSpeed;
	GetCharacterMovement()->JumpZVelocity = NewJumpVelocity;
}
