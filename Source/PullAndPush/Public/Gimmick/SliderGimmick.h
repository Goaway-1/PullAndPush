// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "SliderGimmick.generated.h"

UCLASS()
class PULLANDPUSH_API ASliderGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	ASliderGimmick();

public:	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USceneComponent> DefaultSceneComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Gimmick")
	TObjectPtr<class UTimelineComponent> TimelineComp;

	// Update 'Static Mesh' Location
	UFUNCTION()
	void InterpolateLocation(float DeltaTime);

private:
	UFUNCTION(Server, Reliable)
	void ServerUpdateLocation(const FVector NewLocation);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gimmick")
	TObjectPtr<UCurveVector> SlideCurve;

	UPROPERTY(Replicated)
	FVector PredictedLocation;

	FOnTimelineVector SlideInterpFunction;
	const FName CollisionName = FName("Gimmick");
	float InterpolationSpeed = 5.0f;
};
