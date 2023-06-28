// Fill out your copyright notice in the Description page of Project Settings.

#include "RocketPunch/RPCollisionComponent.h"
#include "Interface/CharacterInterActionHandler.h"

URPCollisionComponent::URPCollisionComponent()
	:
	bIsAlreadyOverlapped(0), MaxKnockBackForce(100000.f), MinKnockBackForce(50000.f)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	OnArrayReset.BindUObject(this, &URPCollisionComponent::ResetOverlapActors);
}
void URPCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
void URPCollisionComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit, AActor* CasterActor, const bool IsPush,const float& ForceAlpha)
{
	// Check if the object is already processed
	if (OverlapActors.Find(OtherActor) || bIsAlreadyOverlapped) return;

	// Event of Push or Pull 
	const FName OtherCompCollsionName = OtherComponent->GetCollisionProfileName();
	float LerpForce = FMath::Lerp(MinKnockBackForce, MaxKnockBackForce, ForceAlpha);
	AActor* TargetActor;
	if (OtherCompCollsionName == "BlockAll" || OtherCompCollsionName == "Pawn")
	{
		if (OtherCompCollsionName == "BlockAll")
		{
			LerpForce *= (IsPush) ? -1 : 1;
			TargetActor = CasterActor;
		}
		else
		{
			LerpForce *= (IsPush) ? 1 : -1;
			TargetActor = OtherActor;
		}
		ApplyPunchImpulseCharacter(HitComponent, TargetActor, LerpForce, IsPush);
	}
	else if (OtherCompCollsionName == "PhysicsActor" || OtherCompCollsionName == "Item")
	{
		if (IsPush)
		{
			ApplyPunchImpulsePrimitiveComponent(OtherComponent, Hit, LerpForce);
		}
		else
		{
			GrapActorToOwner(OtherActor, OtherComponent);
		}
	}
	OnForceReturn.Execute(true);

	// Set Value
	bIsAlreadyOverlapped = true;
	OverlapActors.Add(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("[URPCollisionComponent] Overrlap Type is '%s', Name is '%s'"), *OtherComponent->GetCollisionProfileName().ToString(), *OtherActor->GetName());
}
void URPCollisionComponent::ResetOverlapActors()
{
	/** Reset Grap Object */
	if (GrapActor.IsValid() && GrapUPrimitiveComponent.IsValid()) {
		GrapUPrimitiveComponent.Get()->SetSimulatePhysics(true);
		GrapUPrimitiveComponent.Get()->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		GrapActor.Get()->DetachFromActor(DetachmentRules);
		GrapActor = nullptr;
	}

	bIsAlreadyOverlapped = false;
	OverlapActors.Reset();
}
void URPCollisionComponent::ApplyPunchImpulseCharacter(UPrimitiveComponent* HitComponent, AActor* TargetActor, float ImpulseForce, bool IsPush)
{
	TScriptInterface<class ICharacterInterActionHandler> ActionHandler = TargetActor;
	if (ActionHandler.GetInterface()) {
		FVector ImpulseDirection = HitComponent->GetForwardVector() * ImpulseForce;
		ActionHandler->ApplyPunchImpulse(ImpulseDirection, IsPush);
	}
}
void URPCollisionComponent::ApplyPunchImpulsePrimitiveComponent(UPrimitiveComponent* OtherComponent, const FHitResult& Hit, float ImpulseForce)
{
	FVector ImpulseDirection = -Hit.Normal * (ImpulseForce / 50);
	OtherComponent->AddImpulse(ImpulseDirection, NAME_None, true);
}
void URPCollisionComponent::GrapActorToOwner(AActor* TargetActor, UPrimitiveComponent* OtherComponent)
{
	GrapActor = TargetActor;
	GrapUPrimitiveComponent = OtherComponent;

	if (GrapUPrimitiveComponent.IsValid()) {
		GrapUPrimitiveComponent.Get()->SetSimulatePhysics(false);
		GrapActor.Get()->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
	}
}