// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DeployableItem/SnareDeployableItem.h"
#include "Components/BoxComponent.h"
#include "Interface/CharacterStatHandler.h"

ASnareDeployableItem::ASnareDeployableItem()
	:
	bIsCollision(0)
{
	EventCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("EventCollisionComp"));
	EventCollisionComp->SetBoxExtent(FVector(40.f, 40.f, 40.f));
	EventCollisionComp->SetupAttachment(GetRootComponent());
	EventCollisionComp->SetSimulatePhysics(false);
	EventCollisionComp->SetGenerateOverlapEvents(false);

	CharacterStatModifier.Stat = ECharacterStat::Snare;
	CharacterStatModifier.ChangeDuration = 3.f;
	bIsAutoActive = false;
}
void ASnareDeployableItem::BeginPlay()
{
	Super::BeginPlay();

	EventCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASnareDeployableItem::AddOverlapActors);
}
void ASnareDeployableItem::ActiveDeployableItem()
{
	Super::ActiveDeployableItem();

	CollisionComp->SetSimulatePhysics(false);
	EventCollisionComp->SetGenerateOverlapEvents(true);
	ProjectileMovementComponent->Velocity = FVector::Zero();
}
void ASnareDeployableItem::AddOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsCollision) return;

	// Actor to Bind
	TScriptInterface<class ICharacterStatHandler> ActionHandler = OtherActor;
	if (ActionHandler.GetInterface())
	{
		ActionHandler->EnableStatFlag(CharacterStatModifier.Stat, CharacterStatModifier.ChangeDuration);

		CollisionComp->SetSimulatePhysics(false);
		CollisionComp->SetEnableGravity(false);
		ProjectileMovementComponent->ProjectileGravityScale = 0;
		ProjectileMovementComponent->Velocity = FVector::Zero();
		bIsCollision = true;

		// Destory
		DestoryDeployableItem();
	}
}