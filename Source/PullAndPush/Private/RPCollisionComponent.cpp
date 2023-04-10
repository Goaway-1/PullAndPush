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
			// 시전자가 해당 위치로 이동
			ICollisionActionHandler* ActionHandler = Cast<ICollisionActionHandler>(CasterActor);
			if (ActionHandler) {
				ActionHandler->SetMoveToLocation(Hit.Location);
			}
		}
		else if (OtherCompCollsionName == "PhysicsActor" || OtherCompCollsionName == "Pawn" || OtherCompCollsionName == "Item") {
			// @TODO: 오브젝트를 투사체에 부착하고, 투사체 복귀	 || 도중에 방해요소 존재 시 오브젝트 부착 해제

			// 안되네...
			//GetOwner()->AttachToActor(OtherActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		OnForceReturn.Execute(true);
	}

	OverlapActors.Add(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("[URPCollisionComponent] Overrlap Type is '%s', Name is '%s'"), *OtherComponent->GetCollisionProfileName().ToString(), *OtherActor->GetName());
}
void URPCollisionComponent::ResetOverlapActors()
{
	OverlapActors.Reset();
}
void URPCollisionComponent::KnockBackActor(UPrimitiveComponent* HitComponent, AActor* TargetActor, float ImpulseForce)
{
	ICollisionActionHandler* ActionHandler = Cast<ICollisionActionHandler>(TargetActor);
	if (ActionHandler) {
		FVector ImpulseDirection = HitComponent->GetForwardVector() * ImpulseForce;
		ActionHandler->KnockBackActor(ImpulseDirection);
	}
}
void URPCollisionComponent::KnockBackPrimitiveComponent(UPrimitiveComponent* OtherComponent, const FHitResult& Hit, float ImpulseForce)
{
	FVector ImpulseDirection = -Hit.Normal * (ImpulseForce / 50);
	OtherComponent->AddImpulse(ImpulseDirection, NAME_None, true);
}