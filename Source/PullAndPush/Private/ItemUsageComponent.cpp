// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemUsageComponent.h"

UItemUsageComponent::UItemUsageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}
void UItemUsageComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
void UItemUsageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
void UItemUsageComponent::SpeedUp(UItemDataAsset* ItemData)
{
	UE_LOG(LogTemp, Warning, TEXT("[UItemUsageComponent] SpeedUp "));
}