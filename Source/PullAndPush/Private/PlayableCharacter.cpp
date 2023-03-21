#include "PlayableCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayableCharacter::APlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

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
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void APlayableCharacter::MoveRight(float NewAxisValue) {
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void APlayableCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void APlayableCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

