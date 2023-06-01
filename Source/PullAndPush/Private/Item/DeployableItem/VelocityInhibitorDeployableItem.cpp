// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DeployableItem/VelocityInhibitorDeployableItem.h"
#include "Components/BoxComponent.h"
#include "Interface/CharacterStatHandler.h"

AVelocityInhibitorDeployableItem::AVelocityInhibitorDeployableItem()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetBoxExtent(FVector(200.f,200.f,80.f));
	CollisionComp->SetupAttachment(GetRootComponent());
	CollisionComp->SetSimulatePhysics(false);
	CollisionComp->SetGenerateOverlapEvents(false);

	// Only when out of region will the effect disappear
	CharacterStatModifier.Stat = ECharacterStat::Slow;
	CharacterStatModifier.ChangeDuration = 100.f;
	bIsAutoActive = false;
}
void AVelocityInhibitorDeployableItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AVelocityInhibitorDeployableItem::AddOverlapActors);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AVelocityInhibitorDeployableItem::DeleteOverlapActors);
}
void AVelocityInhibitorDeployableItem::ActiveDeployableItem()
{
	Super::ActiveDeployableItem();

	MeshComp->SetSimulatePhysics(false);
	CollisionComp->SetGenerateOverlapEvents(true);
	ProjectileMovementComponent->Velocity = FVector::Zero();
}
void AVelocityInhibitorDeployableItem::AddOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TScriptInterface<ICharacterStatHandler> CharacterHandler = OtherActor;
	if (CharacterHandler.GetInterface())
	{
		CharacterHandler->EnableStatFlag(CharacterStatModifier.Stat, CharacterStatModifier.ChangeDuration);
	}
}
void AVelocityInhibitorDeployableItem::DeleteOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TScriptInterface<ICharacterStatHandler> CharacterHandler = OtherActor;
	if (CharacterHandler.GetInterface())
	{
		CharacterHandler->DisableStatFlag(CharacterStatModifier.Stat);
	}
}
