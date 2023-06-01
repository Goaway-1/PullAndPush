// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DeployableItem/SnareDeployableItem.h"
#include "Components/BoxComponent.h"
#include "Interface/CharacterStatHandler.h"

ASnareDeployableItem::ASnareDeployableItem()
	:
	bIsCollision(0)
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetBoxExtent(FVector(40.f, 40.f, 40.f));
	CollisionComp->SetupAttachment(GetRootComponent());
	CollisionComp->SetSimulatePhysics(false);
	CollisionComp->SetGenerateOverlapEvents(false);

	CharacterStatModifier.Stat = ECharacterStat::Snare;
	CharacterStatModifier.ChangeDuration = 3.f;
	bIsAutoActive = false;
}
void ASnareDeployableItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASnareDeployableItem::AddOverlapActors);
}
void ASnareDeployableItem::ActiveDeployableItem()
{
	Super::ActiveDeployableItem();

	MeshComp->SetSimulatePhysics(false);
	CollisionComp->SetGenerateOverlapEvents(true);
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

		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetEnableGravity(false);
		ProjectileMovementComponent->ProjectileGravityScale = 0;
		ProjectileMovementComponent->Velocity = FVector::Zero();
		bIsCollision = true;

		// Destory
		DestoryDeployableItem();
	}
}