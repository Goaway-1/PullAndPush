// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCollisionComponent.h"

URPCollisionComponent::URPCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	OnReset.BindUObject(this, &URPCollisionComponent::ResetOverlapActors);
}
void URPCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
void URPCollisionComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit, const bool IsPush)
{
	// Check if the object is already processed
	if (OverlapActors.Find(OtherActor)) return;

	const FName OtherCompCollsionName = OtherComponent->GetCollisionProfileName();

	// TODO : 함수로 구분.
	if (IsPush) {	// Push	
		if (OtherCompCollsionName == "BlockAll") {
			// @TODO: 시전자 뒤로 밀치며, 투사체 복귀 (인터페이스 사용)

			OnForceReturn.Execute(true);
		}
		else if (OtherCompCollsionName == "PhysicsActor" || OtherCompCollsionName == "Item") {
			// @TODO: 오브젝트를 모두 밀침
			FVector ImpulseDirection = -Hit.Normal;
            float ImpulseStrength = 1000.0f;
			OtherComponent->AddImpulse(ImpulseDirection * ImpulseStrength, NAME_None, true);
		}
	}
	else {		// Pull
		if (OtherCompCollsionName == "BlockAll") {			
			// @TODO: 투사체를 고정한 뒤, 시전자가 해당 위치로 이동
		}
		else if (OtherCompCollsionName == "PhysicsActor" || OtherCompCollsionName == "Item") {
			// @TODO: 오브젝트를 투사체에 부착하고, 투사체 복귀			
			// 도중에 방해요소 존재 시 오브젝트 부착 해제
			
			OnForceReturn.Execute(true);
		}
	}

	OverlapActors.Add(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("[URPCollisionComponent] Overrlap Type is '%s', Name is '%s'"), *OtherComponent->GetCollisionProfileName().ToString(), *OtherActor->GetName());
}

void URPCollisionComponent::ResetOverlapActors()
{
	OverlapActors.Reset();
}
