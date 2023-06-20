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
void UInGameInstance::InitSetting(const int8 InMaxRoundCount)
{
	MaxRoundCount = InMaxRoundCount;
	bIsFirstRoundStart = true;
}
bool UInGameInstance::IsAllRoundsFinished()
{
	return (CurrentRoundCount++ < MaxRoundCount) ? false : true;
}
void UInGameInstance::SetPlayersScore(TMap<FString, int8>& Controllers)
{
	for (auto Controller : Controllers)
	{
		if (PlayersScore.Contains(Controller.Key))
		{
			PlayersScore[Controller.Key] += Controller.Value;
		}
		else
		{
			PlayersScore.Add(Controller.Key, Controller.Value);
		}
	}
}

TMap<FString, int8>& UInGameInstance::GetPlayersScore()
{
	return PlayersScore;
}
