// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPCollisionComponent.generated.h"

/// <summary>
/// 충돌한 오브젝트...?
/// </summary>


DECLARE_DELEGATE(FRPCollisionComponentReset)

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
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit, const bool IsPush);

	UFUNCTION()
	void ResetOverlapActors();

	FRPCollisionComponentReset OnReset;

private:
	UPROPERTY(VisibleAnywhere, Category = "OverlapActors")
	TSet<TObjectPtr<AActor>> OverlapActors;
};
