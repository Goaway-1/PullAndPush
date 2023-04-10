// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPCollisionComponent.generated.h"

DECLARE_DELEGATE(FRPCollisionComponentReset)
DECLARE_DELEGATE_OneParam(FRPCollisionComponentForceReturn, const bool&)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PULLANDPUSH_API URPCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URPCollisionComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit, AActor* CasterActor, const bool IsPush, const float& ForceAlpha);

	UFUNCTION()
	void ResetOverlapActors();

	FRPCollisionComponentReset OnArrayReset;			// Reset Overlap Actors
	FRPCollisionComponentForceReturn OnForceReturn;		// Set RocketPunch Force Return

private:
	UPROPERTY(VisibleAnywhere, Category = "OverlapActors")
	TSet<TObjectPtr<AActor>> OverlapActors;

	UPROPERTY(EditAnyWhere, Category = "Info", meta = (AllowPrivateAccess = "true"))
	float MaxKnockBackForce;

	UPROPERTY(EditAnyWhere, Category = "Info", meta = (AllowPrivateAccess = "true"))
	float MinKnockBackForce;

	void KnockBackActor(class UPrimitiveComponent* HitComponent, AActor* TargetActor, float ImpulseForce);
	void KnockBackPrimitiveComponent(class UPrimitiveComponent* OtherComponent, const FHitResult& Hit, float ImpulseForce);
};
