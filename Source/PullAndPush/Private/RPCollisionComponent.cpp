// Fill out your copyright notice in the Description page of Project Settings.

#include "RPCollisionComponent.h"
#include "CollisionActionHandler.h"

URPCollisionComponent::URPCollisionComponent()
	:
	MaxKnockBackForce(100000.f), MinKnockBackForce(50000.f)
{
	PrimaryComponentTick.bCanEverTick = false;

	OnArrayReset.BindUObject(this, &URPCollisionComponent::ResetOverlapActors);
}
void URPCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
void URPCollisionComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit, AActor* CasterActor, const bool IsPush,const float& ForceAlpha)
{
	// Check if the object is already processed
	if (OverlapActors.Find(OtherActor)) return;

	const FName OtherCompCollsionName = OtherComponent->GetCollisionProfileName();

	// Event of Push or Pull 
	if (IsPush) {	
		float LerpForce = FMath::Lerp(MinKnockBackForce, MaxKnockBackForce, ForceAlpha);

		if (OtherCompCollsionName == "BlockAll" || OtherCompCollsionName == "Pawn") {
			// KnockBack Caster Actor & Return RocketPunch
			if (OtherCompCollsionName == "BlockAll") KnockBackActor(HitComponent, CasterActor, -LerpForce);
			else KnockBackActor(HitComponent, OtherActor, LerpForce);

			OnForceReturn.Execute(true);
		}
		else if (OtherCompCollsionName == "PhysicsActor" || OtherCompCollsionName == "Item") {
			// KnockBack All Object
			KnockBackPrimitiveComponent(OtherComponent, Hit, LerpForce);
		}
	}
	else {
		if (OtherCompCollsionName == "BlockAll") {			
			// CasterActor move to hit location
			MoveToLocation(CasterActor,Hit.Location);
		}
		else if (!GrapActor && OtherCompCollsionName == "PhysicsActor" || OtherCompCollsionName == "Pawn" || OtherCompCollsionName == "Item") {
			// Hit object follow the RP
			// @TODO : 도중에 방해요소 존재 시 오브젝트 부착 해제

			if (OtherCompCollsionName == "PhysicsActor") 
			{
				GrapActorToOwnwer(OtherActor, OtherComponent);
			}
			else 
			{
				GrapActorToOwnwer(OtherActor);
			}
		}

		OnForceReturn.Execute(true);
	}

	OverlapActors.Add(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("[URPCollisionComponent] Overrlap Type is '%s', Name is '%s'"), *OtherComponent->GetCollisionProfileName().ToString(), *OtherActor->GetName());
}
void URPCollisionComponent::ResetOverlapActors()
{
	/** Reset Grap Object */
	// Item, Character of Pull
	TScriptInterface<class ICollisionActionHandler> ActionHandler = GrapActor;
	if (ActionHandler.GetInterface()) {
		ActionHandler->SetMoveToActor(nullptr);
		GrapActor = nullptr;
	}

	// PhysicsActor of Pull
	if (IsValid(GrapActor)) {
		GrapUPrimitiveComponent->SetSimulatePhysics(true);
		GrapUPrimitiveComponent->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		GrapActor->DetachFromActor(DetachmentRules);
		GrapActor = nullptr;
	}

	OverlapActors.Reset();
}
void URPCollisionComponent::KnockBackActor(UPrimitiveComponent* HitComponent, AActor* TargetActor, float ImpulseForce)
{
	TScriptInterface<class ICollisionActionHandler> ActionHandler = TargetActor;
	if (ActionHandler.GetInterface()) {
		FVector ImpulseDirection = HitComponent->GetForwardVector() * ImpulseForce;
		ActionHandler->KnockBackActor(ImpulseDirection);
	}
}
void URPCollisionComponent::KnockBackPrimitiveComponent(UPrimitiveComponent* OtherComponent, const FHitResult& Hit, float ImpulseForce)
{
	FVector ImpulseDirection = -Hit.Normal * (ImpulseForce / 50);
	OtherComponent->AddImpulse(ImpulseDirection, NAME_None, true);
}
void URPCollisionComponent::MoveToLocation(AActor* TargetActor, FVector Location)
{
	TScriptInterface<class ICollisionActionHandler> ActionHandler = TargetActor;
	if (ActionHandler.GetInterface()) {
		ActionHandler->SetMoveToLocation(Location);
	}
}
void URPCollisionComponent::GrapActorToOwnwer(AActor* TargetActor, UPrimitiveComponent* OtherComponent)
{
	GrapActor = TargetActor;
	GrapUPrimitiveComponent = OtherComponent;

	if (IsValid(GrapUPrimitiveComponent)) {
		GrapUPrimitiveComponent->SetSimulatePhysics(false);
		GrapActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
	}
	else{
		TScriptInterface<class ICollisionActionHandler> ActionHandler = GrapActor;
		if (ActionHandler.GetInterface()) {
			ActionHandler->SetMoveToActor(GetOwner());
		}
	}
}