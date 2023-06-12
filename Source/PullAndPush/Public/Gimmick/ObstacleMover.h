// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "ObstacleMover.generated.h"

UCLASS()
class PULLANDPUSH_API AObstacleMover : public AActor
{
	GENERATED_BODY()
	
public:	
	AObstacleMover();

public:	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USceneComponent> DefaultSceneComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTimelineComponent> LocationTimelineComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTimelineComponent> RotationTimelineComp;

	void SetInitTimeline();

	// Update 'Static Mesh' Location
	void InterpolateLocation(float DeltaTime);
	void InterpolateRotator(float DeltaTime);

private:
	UFUNCTION(Server, Reliable)
	void ServerUpdateLocation(const FVector NewLocation);

	UFUNCTION(Server, Reliable)
	void ServerUpdateRotation(const FVector NewRotator);

	UFUNCTION()
	void StartMovement();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gimmick")
	TObjectPtr<UCurveVector> LocationSlideCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Gimmick")
	TObjectPtr<UCurveVector> RotationSlideCurve;

	// Time to start Timeline
	UPROPERTY(EditAnywhere, Category = "Gimmick")
	float MovementStartTime;

	UPROPERTY(Replicated)
	FVector PredictedLocation;

	UPROPERTY(Replicated)
	FRotator PredictedRotation;

	FOnTimelineVector SlideLocationInterpFunction;
	FOnTimelineVector SlideRotationInterpFunction;
	const FName MeshCollisionName = FName("Gimmick");
	const float InterpolationSpeed = 5.0f;
};
