#include "Character/PlayableCharacter.h"
#include "Character/AttackComponent.h"
#include "Character/ItemUsageComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Curves/CurveFloat.h"
#include "DrawDebugHelpers.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Runnable/CharacterPropertyRunnable.h"

APlayableCharacter::APlayableCharacter()
	:
	bIsMoveToLocation(false), TargetLocation(FVector(0.f)), StartLocation(FVector(0.f)), MoveToLocationSpeed(5000.f), bIsMoveToActor(false), MoveTargetActor(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComp"));
	ItemUsageComp = CreateDefaultSubobject<UItemUsageComponent>(TEXT("ItemUsageComp"));

	// Camera Setting
	InitSpringArm(SpringArmComp, 450.f, FVector(0.f, 0.f, 60.f));
	CameraComp->SetupAttachment(SpringArmComp);
	bUseControllerRotationYaw = false;

	// ZoomIn & Out
	ZoomTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomTimeline"));
	ZoomInterpFunction.BindUFunction(this, FName("UpdateSpringArmLength"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,700.f,0.f);
	GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed;
}
APlayableCharacter::~APlayableCharacter()
{
	if (PropertyRunnable != nullptr)
	{
		PropertyRunnable->Stop();
	}
}
void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitZoomTimeLine();

	/** Thread for Character Properties... */
	PropertyRunnable = new FCharacterPropertyRunnable();
	PropertyRunnable->CompleteSignature.BindUObject(this, &APlayableCharacter::ActiveMovementSpeed);
	SetMovementSpeed(false, DefaultMoveSpeed);
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
	InitEnhancedInput();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(!IsValid(MoveAction) || !IsValid(LookAction) || !IsValid(JumpAction) || !IsValid(RPAction)) return;

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, "Move");
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, "Look");
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, "Jump");

		EnhancedInputComponent->BindAction(RPAction, ETriggerEvent::Started, this, "TryLaunch");
		EnhancedInputComponent->BindAction(RPAction, ETriggerEvent::Completed, this, "EndLaunch");
	}
}
void APlayableCharacter::SetPlayerAttackCondition(const bool& IsCharging)
{
	PlayerAttackCondition = (IsCharging) ? EPlayerAttackCondition::EPAC_Charging : EPlayerAttackCondition::EPAC_Idle;

	ZoomInOut();
	SetMovementSpeed(IsCharging);
	ActiveMovementSpeed(IsCharging);	
}
void APlayableCharacter::InitEnhancedInput()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			SubSystem->AddMappingContext(DefaultContext, 0);
	}
}
void APlayableCharacter::Move(const FVector2D& Value)
{
	MoveForward(Value.Y);
	MoveRight(Value.X);
}
void APlayableCharacter::Look(const FVector2D& AxisValue) {
	LookUp(AxisValue.Y);
	Turn(AxisValue.X);
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
void APlayableCharacter::TryLaunch(const FVector2D& Value)
{
	if (AttackComp->TryLaunch()) {
		// Set Attack Type if can Charnging
		bIsPush = (Value.X > 0) ? false : true;
	}
}
void APlayableCharacter::EndLaunch()
{
	AttackComp->EndLaunch(bIsPush);
}
void APlayableCharacter::SetMovementSpeed(const bool& IsCharging, const float& NewMoveSpeed)
{
	ensure(PropertyRunnable);

	// Is Item Activated
	if (NewMoveSpeed > 0.f || NewMoveSpeed < 0.f) {
		float Speed = CurrentMoveSpeed + NewMoveSpeed;

		// Thread for Character Properties
		PropertyRunnable->SetMoveSpeed(Speed);
	}
}
void APlayableCharacter::ActiveMovementSpeed(const bool& IsCharging)
{
	ensure(PropertyRunnable);

	// Update CurrentMoveSpeed (in Local)
	CurrentMoveSpeed = PropertyRunnable->GetMoveSpeed();
	float Speed = CurrentMoveSpeed;
	float Velocity = MaxJumpVelocity;

	if (IsCharging || GetPlayerAttackCondition() == EPlayerAttackCondition::EPAC_Charging) {
		Speed /= 2;
		Velocity /= 2;
	}

	// Set Character Movmemt Properties...
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	GetCharacterMovement()->JumpZVelocity = Velocity;
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
void APlayableCharacter::ZoomInOut()
{
	if (GetPlayerAttackCondition() == EPlayerAttackCondition::EPAC_Charging) {
		ZoomTimeline->Play();
		ZoomTimeline->SetPlayRate(1.f);
	}
	else {
		ZoomTimeline->Reverse();
		ZoomTimeline->SetPlayRate(2.f);
	}
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
void APlayableCharacter::PickUpItem(UItem* ItemData)
{
	// @TODO : Controller에 UI의 정보[델리게이트]와 ItemUsageComponent에 Item정보[함수 호출]를 넘겨준다. (데이터를 넘겨주는 역할)
	ItemUsageComp->PickUpItem(ItemData);
}
void APlayableCharacter::RocketPunchAlphaSpeed(const float& AlphaSpeed)
{
	AttackComp->SetRPAlphaSpeed(AlphaSpeed);
}
void APlayableCharacter::RocketPunchAlphaRange(const float& AlphaRange)
{
	AttackComp->SetRPAlphaRange(AlphaRange);
}
void APlayableCharacter::RocketPunchAlphaSize(const float& AlphaSize)
{
	AttackComp->SetRPAlphaSize(AlphaSize);
}
