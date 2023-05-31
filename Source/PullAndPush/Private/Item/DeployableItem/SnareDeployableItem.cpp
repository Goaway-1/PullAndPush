// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DeployableItem/SnareDeployableItem.h"
#include "Components/BoxComponent.h"
#include "Interface/CharacterStatHandler.h"

ASnareDeployableItem::ASnareDeployableItem()
	:
	bIsCollision(0)
{
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName(CollisionName);

	CharacterStatModifier.Stat = ECharacterStat::Snare;
	CharacterStatModifier.ChangeDuration = 3.f;
}
void ASnareDeployableItem::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->OnComponentHit.AddDynamic(this, &ASnareDeployableItem::OnHit);
}
void ASnareDeployableItem::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if(bIsCollision) return;

	// Actor to Bind
	TScriptInterface<class ICharacterStatHandler> ActionHandler = OtherActor;
	if (ActionHandler.GetInterface())
	{
		ActionHandler->EnableStatFlag(CharacterStatModifier.Stat, CharacterStatModifier.ChangeDuration);

		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetEnableGravity(false);
		ProjectileMovementComponent->ProjectileGravityScale = 0;
		ProjectileMovementComponent->Velocity = FVector::Zero();
		bIsCollision = true;

		// Destory
		DestoryDeployableItem();
	}
}