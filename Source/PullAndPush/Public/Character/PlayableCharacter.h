// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "GameFramework/Character.h"
#include "Interface/CharacterInterActionHandler.h"
#include "Interface/CharacterPickupHandler.h"
#include "Interface/CharacterStatHandler.h"
#include "PlayableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PULLANDPUSH_API APlayableCharacter : public ACharacter, public ICharacterInterActionHandler, public ICharacterPickupHandler, public ICharacterStatHandler
{
	GENERATED_BODY()

/** Default */
#pragma region DEFAULT
public:
	APlayableCharacter();
	~APlayableCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UAttackComponent> AttackComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UItemUsageComponent> ItemUsageComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStatComponent> StatComp;

private:
	UFUNCTION()
	void Move(const FVector2D& Value);

	UFUNCTION()
	void Look(const FVector2D& AxisValue);

	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	// Try to set Movement Speed (Call Item or Charging)
	virtual void SetMovementSpeed(const float NewMoveSpeed = 0.f) override;

	// Actually set Movement Speed
	void ActiveMovementSpeed();

	UFUNCTION(Server, Reliable)
	void ServerActiveMovementSpeed(const float InSpeed, const float InJump);

	TObjectPtr<class APlayableController> PlayableController;

private:
	UFUNCTION()
	void UpdateAimPitch();

	UFUNCTION(Server, Reliable)
	void ServerUpdateAimPitch();

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float AimPitch;

#pragma endregion

/** Charging */
#pragma region CHARGING&ZOOM
/** Charging */
private:
	UFUNCTION()
	void TryLaunch(const FVector2D& Value);

	UFUNCTION()
	void EndLaunch();

	// Set Spring Arm
	UFUNCTION()
	void UpdateSpringArmLength(const float NewArmLength);
	void SetPlayerView();

	void InitSpringArm(USpringArmComponent* SpringArm, const float& NewTargetArmLength, const FVector& NewSocketOffset);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EPlayerAttackCondition GetPlayerAttackCondition() { return PlayerAttackCondition; }

	UFUNCTION(BlueprintCallable)
	virtual void SetPlayerAttackCondition(const bool IsCharging) override;

	UFUNCTION(Server, Reliable)
	void ServerSetPlayerAttackCondition(const bool IsCharging);
public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly,Category = "Condition")
	EPlayerAttackCondition PlayerAttackCondition;

	uint8 bIsPush : 1;

/** Zoom */
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<class UAimingComponent> AimingComp;

#pragma endregion

/** Collision Hit Event Of Another Actor */
#pragma region COLLISION_HIT
public:
	virtual void KnockBackActor(const FVector& DirVec) override;
	virtual void SetMoveToLocation(const FVector& HitVector) override;
	virtual void SetMoveToActor(AActor* TargetActor) override;

private:
	void UpdateMoveToLocation(float DeltaTime);
	void UpdateMoveToActor();

private:
	uint8 bIsMoveToLocation : 1;
	FVector TargetLocation;
	FVector StartLocation;
	const float StopToMoveDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "Speed", Meta = (AllowPrivateAccess = true))
	float MoveToLocationSpeed;

	/** Event of Collision Hit */
	UPROPERTY(VisibleAnywhere)
	uint8 bIsMoveToActor : 1;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor> MoveTargetActor;
#pragma endregion

/** Enhanced Input */
#pragma region ENHANCED INPUT
protected:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultContext;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputAction> RPAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputAction> ItemUseAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputAction> ShowItemInfoAction;

private:
	void InitEnhancedInput();
#pragma endregion

/** Item */
#pragma region ITEM
public:
	virtual void PickUpItem(class UItemData* ItemData) override;

	UFUNCTION(Client, Reliable)
	void ClientPickUpItem(class UItemData* ItemData);

	// Set Alpha Value Affected By Item
	virtual void SetRocketPunchSpeed(const float& deltaspeed) override;
	virtual void SetRocketPunchRange(const float& deltarange) override;
	virtual void SetRocketPunchScale(const float& deltasize) override;

	virtual float GetRocketPunchSpeed() override;
	virtual float GetRocketPunchRange() override;
	virtual float GetRocketPunchScale() override;

private:
	UFUNCTION()
	void UseActiveItem();

	UFUNCTION()
	void ChangeVisibleItemInfo(const FInputActionValue& Value);
#pragma endregion

/** Stat */
#pragma region STAT
public:
	virtual void EnableStatFlag(ECharacterStat InFlag, float ChangeDuration) override;
	virtual void DisableStatFlag(ECharacterStat InFlag) override;
	virtual bool IsStatFlagSet(ECharacterStat InFlag) override;
#pragma endregion

};
