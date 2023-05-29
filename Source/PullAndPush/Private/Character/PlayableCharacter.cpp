#include "Character/PlayableCharacter.h"
#include "Character/AttackComponent.h"
#include "Character/ItemUsageComponent.h"
#include "Character/AimingComponent.h"
#include "Character/StatComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/PlayableController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

APlayableCharacter::APlayableCharacter()
	:
	bIsMoveToLocation(false), TargetLocation(FVector(0.f)), StartLocation(FVector(0.f)), MoveToLocationSpeed(5000.f), bIsMoveToActor(false), MoveTargetActor(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComp"));
	ItemUsageComp = CreateDefaultSubobject<UItemUsageComponent>(TEXT("ItemUsageComp"));
	AimingComp = CreateDefaultSubobject<UAimingComponent>(TEXT("AimingComp"));
	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));	

	// Camera Setting
	InitSpringArm(SpringArmComp, 450.f, FVector(0.f, 0.f, 60.f));
	CameraComp->SetupAttachment(SpringArmComp);
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,700.f,0.f);
	GetCharacterMovement()->SetIsReplicated(true);
}
APlayableCharacter::~APlayableCharacter()
{

}
void APlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}
void APlayableCharacter::UnPossessed()
{
	// Unmap from inventory source
	if (PlayableController)
	{
		ItemUsageComp->GetItemWidgetUpdateDelegate().Unbind();
	}
}
void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	/** Bind if MoveSpeed is Changed */
	StatComp->OnMoveSpeedChanged.BindUObject(this, &APlayableCharacter::ActiveMovementSpeed);

	/** Set Item Widget */
	PlayableController = Cast<APlayableController>(GetController());
	if (PlayableController && ItemUsageComp)
	{
		ItemUsageComp->GetItemWidgetUpdateDelegate().BindUObject(PlayableController, &APlayableController::UpdateItemUI);
	}
}
void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If Hit Event is Called.
	UpdateMoveToLocation(DeltaTime);
	UpdateMoveToActor();

	UpdateAimPitch();
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, "Jump");
		EnhancedInputComponent->BindAction(ItemUseAction, ETriggerEvent::Started, this, "UseActiveItem");

		EnhancedInputComponent->BindAction(RPAction, ETriggerEvent::Started, this, "TryLaunch");
		EnhancedInputComponent->BindAction(RPAction, ETriggerEvent::Completed, this, "EndLaunch");

		EnhancedInputComponent->BindAction(ShowItemInfoAction, ETriggerEvent::Started, this, "ChangeVisibleItemInfo");
		EnhancedInputComponent->BindAction(ShowItemInfoAction, ETriggerEvent::Completed, this, "ChangeVisibleItemInfo");
	}
}
void APlayableCharacter::SetPlayerAttackCondition(const bool IsCharging)
{
	PlayerAttackCondition = (IsCharging) ? EPlayerAttackCondition::EPAC_Charging : EPlayerAttackCondition::EPAC_Idle;
	ServerSetPlayerAttackCondition(IsCharging);

	AimingComp->ZoomInOut(IsCharging);
	SetPlayerView();
	SetMovementSpeed(); 
}
void APlayableCharacter::ServerSetPlayerAttackCondition_Implementation(const bool IsCharging)
{
	PlayerAttackCondition = (IsCharging) ? EPlayerAttackCondition::EPAC_Charging : EPlayerAttackCondition::EPAC_Idle;
	SetPlayerView();
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
	if(StatComp->IsStatFlagSet(ECharacterStat::Stun)) return;

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
void APlayableCharacter::UpdateAimPitch()
{
	if (HasAuthority() && GetPlayerAttackCondition() == EPlayerAttackCondition::EPAC_Charging)
	{
		ServerUpdateAimPitch();
	}
}
void APlayableCharacter::ServerUpdateAimPitch_Implementation()
{
	FRotator TargetRot = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());
	AimPitch = TargetRot.Pitch;
}
void APlayableCharacter::TryLaunch(const FVector2D& Value)
{
	// If item exists, throw it
	if (ItemUsageComp->GetIsReadyToThrow()) {
		ItemUsageComp->ThrowDeployableItem();
		return;
	}

	// Launch RocketPunch
	if (AttackComp->TryLaunch()) {
		// Set Attack Type if can Charnging
		bIsPush = (Value.X > 0) ? false : true;
	}
}
void APlayableCharacter::EndLaunch()
{
	AttackComp->EndLaunch(bIsPush);
}
void APlayableCharacter::SetMovementSpeed(const float NewMoveSpeed)
{	
	// Set Move Speed
	if (!StatComp->ModifyMoveSpeed(NewMoveSpeed))
	{
		ActiveMovementSpeed();
	}
}
void APlayableCharacter::ActiveMovementSpeed()
{
	float Speed = StatComp->GetMoveSpeed();
	float Velocity = StatComp->GetJumpVelocity();

	if (GetPlayerAttackCondition() == EPlayerAttackCondition::EPAC_Charging) 
	{
		Speed /= 2;
		Velocity /= 2;
	}

	// Set Character Movmemt Properties...
	if (GetLocalRole() ==  ROLE_AutonomousProxy)
	{
		ServerActiveMovementSpeed(Speed, Velocity);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = Speed;
		GetCharacterMovement()->JumpZVelocity = Velocity;
	}
}
void APlayableCharacter::ServerActiveMovementSpeed_Implementation(const float InSpeed, const float InJump)
{
	GetCharacterMovement()->MaxWalkSpeed = InSpeed;
	GetCharacterMovement()->JumpZVelocity = InJump;
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
	bUseControllerRotationYaw = (GetPlayerAttackCondition() == EPlayerAttackCondition::EPAC_Charging) ? true : false;
	GetCharacterMovement()->bOrientRotationToMovement = (GetPlayerAttackCondition() == EPlayerAttackCondition::EPAC_Charging) ? false : true;
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
void APlayableCharacter::UpdateMoveToLocation(float DeltaTime)
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
void APlayableCharacter::UpdateMoveToActor()
{
	if (bIsMoveToActor && MoveTargetActor.IsValid()) {
		SetActorLocation(MoveTargetActor.Get()->GetActorLocation());
	}
}
void APlayableCharacter::PickUpItem(UItemData* ItemData)
{
	if (HasAuthority())
	{
		ClientPickUpItem(ItemData);
	}
}
void APlayableCharacter::ClientPickUpItem_Implementation(UItemData* ItemData)
{
	ItemUsageComp->PickUpItem(ItemData);
}
void APlayableCharacter::UseActiveItem() {
	ItemUsageComp->TryToUseActiveItem();
}
void APlayableCharacter::ChangeVisibleItemInfo(const FInputActionValue& Value)
{
	bool bActiveItemVisible = Value.Get<bool>();
	PlayableController->ChangeVisibleItemInfo(bActiveItemVisible);
}
void APlayableCharacter::EnableStatFlag(ECharacterStat InFlag, float ChangeDuration)
{
	StatComp->EnableStatFlag(InFlag, ChangeDuration);
}
void APlayableCharacter::DisableStatFlag(ECharacterStat InFlag)
{
	StatComp->DisableStatFlag(InFlag);
}
bool APlayableCharacter::IsStatFlagSet(ECharacterStat InFlag)
{
	return (StatComp) ? StatComp->IsStatFlagSet(InFlag) : false;
}
void APlayableCharacter::SetRocketPunchSpeed(const float& DeltaSpeed)
{ 
	StatComp->SetRocketPunchSpeed(DeltaSpeed); 
}
void APlayableCharacter::SetRocketPunchRange(const float& DeltaRange)
{ 
	StatComp->SetRocketPunchRange(DeltaRange); 
}
void APlayableCharacter::SetRocketPunchScale(const float& DeltaSize)
{ 
	StatComp->SetRocketPunchScale(DeltaSize); 
}
float APlayableCharacter::GetRocketPunchSpeed()
{ 
	return StatComp->GetRocketPunchSpeed(); 
}
float APlayableCharacter::GetRocketPunchRange()
{ 
	return StatComp->GetRocketPunchRange(); 
}
float APlayableCharacter::GetRocketPunchScale()
{ 
	return StatComp->GetRocketPunchScale(); 
}
void APlayableCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APlayableCharacter, PlayerAttackCondition, COND_SkipOwner);
	DOREPLIFETIME(APlayableCharacter, AimPitch);
}