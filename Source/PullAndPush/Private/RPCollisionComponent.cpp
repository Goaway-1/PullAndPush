// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCollisionComponent.h"

URPCollisionComponent::URPCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// TODO : 초기화 바인딩
	OnReset.BindUObject(this, &URPCollisionComponent::ResetOverlapActors);
}
void URPCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
void URPCollisionComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit, const bool IsPush)
{
	// TODO : 충돌 시 1회만 진행하고, Movment가 끝나면 복귀...
	// 현재 공격이 무엇인지 사용.
	
	if (OverlapActors.Find(OtherActor)) return;

	OverlapActors.Add(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("[URPCollisionComponent] Overrlap Type is '%s', Name is '%s'"), *OtherComponent->GetCollisionProfileName().ToString(), *OtherActor->GetName());
}

void URPCollisionComponent::ResetOverlapActors()
{
	OverlapActors.Reset();
}
