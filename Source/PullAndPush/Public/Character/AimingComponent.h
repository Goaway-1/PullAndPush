// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "AimingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PULLANDPUSH_API UAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAimingComponent();

protected:
	virtual void BeginPlay() override;

public:
	void InitZoomTimeLine();
	void ZoomInOut(const bool IsZoom);

	FORCEINLINE FOnTimelineFloat GetFOnTimelineFloat() { return ZoomInterpFunction; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<UTimelineComponent> ZoomTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCurveFloat> ZoomCurve;

	FOnTimelineFloat ZoomInterpFunction;
};
