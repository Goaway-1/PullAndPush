// Fill out your copyright notice in the Description page of Project Settings.

#include "RPCollisionComponent.h"
#include "CollisionActionHandler.h"

URPCollisionComponent::URPCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	OnArrayReset.BindUObject(this, &URPCollisionComponent::ResetOverlapActors);
}
void URPCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
void URPCollisionComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit, AActor* CasterActor, const bool IsPush)
{
	// Check if the object is already processed
	if (OverlapActors.Find(OtherActor)) return;

	const FName OtherCompCollsionName = OtherComponent->GetCollisionProfileName();

	// TODO : �Լ��� ����.
	if (IsPush) {	// Push	
		if (OtherCompCollsionName == "BlockAll") {
			// test ������ �ڷ� ��ġ��, ����ü ���� 
			ICollisionActionHandler* ActionHandler = Cast<ICollisionActionHandler>(CasterActor);
			if (ActionHandler) {
				ActionHandler->TestAction();
			}

			OnForceReturn.Execute(true);
		}
		else if (OtherCompCollsionName == "PhysicsActor" || OtherCompCollsionName == "Item") {
			// ������Ʈ�� ��� ��ħ
			FVector ImpulseDirection = -Hit.Normal;
            float ImpulseStrength = 1000.0f;
			OtherComponent->AddImpulse(ImpulseDirection * ImpulseStrength, NAME_None, true);
		}
	}
	else {		// Pull
		if (OtherCompCollsionName == "BlockAll") {			
			// @TODO: ����ü�� ������ ��, �����ڰ� �ش� ��ġ�� �̵�
		}
		else if (OtherCompCollsionName == "PhysicsActor" || OtherCompCollsionName == "Item") {
			// @TODO: ������Ʈ�� ����ü�� �����ϰ�, ����ü ����			
			// ���߿� ���ؿ�� ���� �� ������Ʈ ���� ����
			
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
