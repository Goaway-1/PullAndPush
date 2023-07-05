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
	DurationInFlyMode(0.3f), bIsKnockBack(0)
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

	/** Binding Create Item & Stat Widget */
	PlayableController = Cast<APlayableController>(GetController());
	if (PlayableController && ItemUsageComp && StatComp)
	{
		ItemUsageComp->GetItemWidgetUpdateDelegate().BindUObject(PlayableController, &APlayableController::UpdateItemUI);
		StatComp->OnUpdateStatWidget.BindUObject(PlayableController, &APlayableController::UpdateStatUI);
	}
}
void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAimPitch();
	CheckCollisionWithWall();
}
void APlayableCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	/** Notify GameMode that the player is dropped */
	APlayableController* NewPlayableController = Cast<APlayableController>(GetController());
	if (NewPlayableController)
	{
		NewPlayableController->PlayerFellOutOfWorld();
	}

	// Disable Character..
	ClearAllTimer();
	Super::FellOutOfWorld(dmgType);
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
	if(!IsCanMove()) return;

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
	if(!IsCanAttack()) return;

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

	// if Character is Charging
	if (GetPlayerAttackCondition() == EPlayerAttackCondition::EPAC_Charging) 
	{
		Speed /= 2;
		Velocity /= 2;
	}

	// if Character Stat is Slow..
	if (StatComp->IsStatFlagSet(ECharacterStat::Slow))
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
void APlayableCharacter::ApplyPunchImpulse(const FVector& DirVec, bool IsPush)
{
	// If the character is on the ground
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}

	bIsKnockBack = IsPush;
	HitedVector = DirVec;
	GetCharacterMovement()->AddImpulse(DirVec);
	GetWorld()->GetTimerManager().ClearTimer(MovementModeHandle);
	GetWorld()->GetTimerManager().SetTimer(MovementModeHandle, this, &APlayableCharacter::ResetMovementMode, DurationInFlyMode, false);
}
void APlayableCharacter::ApplyObstacleImpulse(const FVector& DirVec)
{
	GetCharacterMovement()->AddImpulse(DirVec);
}
void APlayableCharacter::ResetMovementMode()
{
	if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	bIsKnockBack = false;
}
void APlayableCharacter::CheckCollisionWithWall()
{
	if (bIsKnockBack)
	{
		// Find objects within the sphere
		FHitResult OutHit;
		FCollisionObjectQueryParams CollisionObjectQueryParams;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(RocketPunch), false, this);
		CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		Params.AddIgnoredActor(this);

		// Check Hited
		const FVector OriginPos = GetActorLocation() + (HitedVector.GetSafeNormal() * HitedCheckPos);
		const float CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
		const float CapsuleHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 15.f;
		bool HitDetected = GetWorld()->SweepSingleByObjectType(OutHit, OriginPos, OriginPos, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHeight), Params);
		if (HitDetected)
		{
			EnableStatFlag(ECharacterStat::Stun, KnockBackStunDuration);
			bIsKnockBack = false;
		}

		// For log
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red; 
		DrawDebugCapsule(GetWorld(), OriginPos, CapsuleHeight, CapsuleRadius, FQuat::Identity, DrawColor, false, 0.1f);
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
	ServerEnableStatFlag(InFlag, ChangeDuration);
}
void APlayableCharacter::DisableStatFlag(ECharacterStat InFlag)
{
	StatComp->DisableStatFlag(InFlag);
}
bool APlayableCharacter::IsStatFlagSet(ECharacterStat InFlag)
{
	return (StatComp) ? StatComp->IsStatFlagSet(InFlag) : false;
}
void APlayableCharacter::ServerEnableStatFlag_Implementation(ECharacterStat InFlag, float ChangeDuration)
{
	StatComp->EnableStatFlag(InFlag, ChangeDuration);

	MultiEnableStatFlag(InFlag, ChangeDuration);
}
void APlayableCharacter::MultiEnableStatFlag_Implementation(ECharacterStat InFlag, float ChangeDuration)
{
	StatComp->EnableStatFlag(InFlag, ChangeDuration);
}
bool APlayableCharacter::IsCanMove()
{
	return (StatComp->IsStatFlagSet(ECharacterStat::Stun) || StatComp->IsStatFlagSet(ECharacterStat::Snare)) ? false : true;
}
bool APlayableCharacter::IsCanAttack()
{
	return (StatComp->IsStatFlagSet(ECharacterStat::Stun)) ? false : true;
}
void APlayableCharacter::SetPassiveStat(FPassiveStat InPassiveStat)
{
	StatComp->SetPassiveStat(InPassiveStat);
}
FPassiveStat APlayableCharacter::GetPassiveStat()
{
	return StatComp->GetPassiveStat();
}
void APlayableCharacter::ClearAllTimer()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(ItemUsageComp);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(StatComp);
}
void APlayableCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APlayableCharacter, PlayerAttackCondition, COND_SkipOwner);
	DOREPLIFETIME(APlayableCharacter, AimPitch);
}