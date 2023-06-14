// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/InGameInstance.h"

UInGameInstance::UInGameInstance()
	:
	bIsFirstRoundStart(0), MaxRoundCount(0), CurrentRoundCount(1)
{

}
bool UInGameInstance::IsFirstRoundStart()
{
	return bIsFirstRoundStart;
}
void UInGameInstance::InitSetting(int InMaxRoundCount)
{
	MaxRoundCount = InMaxRoundCount;
	bIsFirstRoundStart = true;
}
bool UInGameInstance::IsAllRoundsFinished()
{
	return (CurrentRoundCount++ < MaxRoundCount) ? false : true;
}
