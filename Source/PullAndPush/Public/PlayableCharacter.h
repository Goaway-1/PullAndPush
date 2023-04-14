// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CollisionActionHandler.h"
#include "Item/PickupActionHandler.h"
#include "Components/TimelineComponent.h"
#include "PlayableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class EPlayerAttackCondition : uint8 {
	EPAC_Idle = 0		UMETA(DisplayName = "Idle"),
	EPAC_Charging		UMETA(DisplayName = "Charging")
};

UCLASS()
class PULLANDPUSH_API APlayableCharacter : public ACharacter, public ICollisionActionHandler, public IPickupActionHandler
{
	GENERATED_BODY()

public:
	APlayableCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EPlayerAttackCondition GetPlayerAttackCondition() {return PlayerAttackCondition;}
	FORCEINLINE void SetPlayerAttackCondition(const EPlayerAttackCondition& NewPlayerAttackCondition) { PlayerAttackCondition = NewPlayerAttackCondition;}
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

	UFUNCTION()
	void TryLaunch(const FVector2D& Value);

	UFUNCTION()
	void EndLaunch();

	/** Charging */
	UPROPERTY(VisibleAnywhere, Category = "Condition")
	EPlayerAttackCondition PlayerAttackCondition;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCurveFloat> ZoomCurve;

	FOnTimelineFloat ZoomInterpFunction;

	uint8 bIsPush : 1;

	UFUNCTION()
	void UpdateSpringArmLength(const float NewArmLength);

	void SetMovementSpeed(const float& NewMoveSpeed, const float& NewJumpVelocity);
	void InitSpringArm(USpringArmComponent* SpringArm, const float& NewTargetArmLength, const FVector& NewSocketOffset);
	void SetPlayerView();
	void InitZoomTimeLine();
	void ZoomInOut(const EPlayerAttackCondition NewCondition);

public:
	/** Event of Collision Hit */
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

	/** Enhanced Input */
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

	/** Item */
public:
	virtual void SpeedUp(class UItemDataAsset* ItemData) override;
};
