#include "RocketPunch/RPCollisionComponent.h"
#include "Interface/CharacterInterActionHandler.h"
#include "GameData/CharacterStat.h"
#include "Net/UnrealNetwork.h"

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
void URPCollisionComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit, AActor* CasterActor, const bool IsPush, const bool IsServerRP, const float& ForceAlpha, FPassiveStat InPassiveStat)
{
	// Check if the object is already processed
	if (OverlapActors.Find(OtherActor) || bIsAlreadyOverlapped) return;

	// Event of Push or Pull 
	if (IsServerRP)
	{
		const FName OtherCompCollsionName = OtherComponent->GetCollisionProfileName();
		float LerpForce = FMath::Lerp(MinKnockBackForce, MaxKnockBackForce + InPassiveStat.RPForce, ForceAlpha);
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
		OverlapActors.Add(OtherActor);
		UE_LOG(LogTemp, Warning, TEXT("[URPCollisionComponent] Overrlap Type is '%s', Name is '%s', Force is '%f'"), *OtherComponent->GetCollisionProfileName().ToString(), *OtherActor->GetName(), LerpForce);
	}
	bIsAlreadyOverlapped = true;
	OnForceReturn.ExecuteIfBound(true);
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
void URPCollisionComponent::OnRep_ChangeGrapActor()
{
	if (GrapActor.IsValid() && GrapUPrimitiveComponent.IsValid()) {
		GrapUPrimitiveComponent.Get()->SetSimulatePhysics(false);
		GrapActor.Get()->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
	}
}
void URPCollisionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URPCollisionComponent, GrapActor);
	DOREPLIFETIME(URPCollisionComponent, GrapUPrimitiveComponent);
}