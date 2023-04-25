// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CollisionActionHandler.h"
#include "Interface/PickupActionHandler.h"
#include "Interface/CharacterPropertyHandler.h"
#include "Components/TimelineComponent.h"
#include "PlayableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PULLANDPUSH_API APlayableCharacter : public ACharacter, public ICollisionActionHandler, public IPickupActionHandler, public ICharacterPropertyHandler
{
	GENERATED_BODY()

/** Default */
public:
	APlayableCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EPlayerAttackCondition GetPlayerAttackCondition() {return PlayerAttackCondition;}

	UFUNCTION(BlueprintCallable)
	virtual void SetPlayerAttackCondition(const EPlayerAttackCondition& NewPlayerAttackCondition) override;

	virtual void SetMovementSpeed(const bool& IsCharging, const float& NewMoveSpeed = 0.f) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UAttackComponent> AttackComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UItemUsageComponent> ItemUsageComp;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<UTimelineComponent> ZoomTimeline;

private:
	UFUNCTION()
	void Move(const FVector2D& Value);

	UFUNCTION()
	void Look(const FVector2D& AxisValue);

	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	// Move properties..
	std::atomic<float> CurrentMoveSpeed;
	const float DefaultMoveSpeed = 600.f;
	const float MaxJumpVelocity = 420.f;

/** Charging */
#pragma region CHARGING
private:
	UFUNCTION()
	void TryLaunch(const FVector2D& Value);

	UFUNCTION()
	void EndLaunch();

	UPROPERTY(VisibleAnywhere, Category = "Condition")
	EPlayerAttackCondition PlayerAttackCondition;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCurveFloat> ZoomCurve;

	FOnTimelineFloat ZoomInterpFunction;
	uint8 bIsPush : 1;

	UFUNCTION()
	void UpdateSpringArmLength(const float NewArmLength);

	void InitSpringArm(USpringArmComponent* SpringArm, const float& NewTargetArmLength, const FVector& NewSocketOffset);
	void SetPlayerView();
	void InitZoomTimeLine();

	void ZoomInOut();
#pragma endregion

/** Collision Hit Event Of Another Actor */
#pragma region COLLISION_HIT
public:
	virtual void KnockBackActor(const FVector& DirVec) override;
	virtual void SetMoveToLocation(const FVector& HitVector) override;
	void MoveToLocation(float DeltaTime);

	virtual void SetMoveToActor(AActor* TargetActor) override;
	void MoveToActor();

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
	TObjectPtr<AActor> MoveTargetActor;
#pragma endregion

/** Enhanced Input */
#pragma region ENHANCED INPUT
public:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> RPAction;

private:
	void InitEnhancedInput();
#pragma endregion

/** Item */
public:
	virtual void PickUpItem(class UItem* ItemData) override;
};
