#include "Game/InGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"

UInGameInstance::UInGameInstance()
{
	ResetData();
}
bool UInGameInstance::IsAllRoundsFinished()
{
	return (CurrentRoundCount++ < MaxRoundCount) ? false : true;
}
void UInGameInstance::ResetData()
{
	CurrentRoundCount = 1;
	TotalPlayerCount = 0;
	PlayersScore.Reset();
	PlayerName = FText::GetEmpty();
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
void UInGameInstance::TravelLevel(ELevelType LevelType)
{
	/*if (LevelType != ELevelType::ELT_Main)
	{
		GetWorld()->ServerTravel(GetLevelPathOfEnum(LevelType));
	}
	else
	{
		ResetData();
		UGameplayStatics::OpenLevel(this, MainLevelName);
	}*/

	switch (LevelType)
	{
	case ELevelType::ELT_Main:
		UGameplayStatics::OpenLevel(this, MainLevelName);
		break;
	case ELevelType::ELT_Lobby:
		GetWorld()->ServerTravel(LobbyLevelName);
		break;
	case ELevelType::ELT_InGame:
		GetWorld()->ServerTravel(GetRandomLevelName());
		break;
	case ELevelType::ELT_Result:
		GetWorld()->ServerTravel(ResultLevelName);
		break;
	}
}
FString UInGameInstance::GetRandomLevelName()
{
	// Get Random Level used by AssetManager
	/*UAssetManager& Manager = UAssetManager::Get();
	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("Map"), Assets);
	ensure(0 < Assets.Num());

	FString AssetName;
	if (Assets.Num() >= 1)
	{
		int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
		FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
		AssetName = LevelDirectoryPath + AssetPtr.GetAssetName();
	}*/
	// @TODO : 이슈.. 실제로 플레이 하면 안됌

	FString AssetName;
	AssetName = LevelDirectoryPath + "Level1?listen";

	return AssetName;
}
FString UInGameInstance::GetLevelPathOfEnum(ELevelType LevelType)
{
	switch (LevelType)
	{
	case ELevelType::ELT_Lobby:
		return LobbyLevelName;
	case ELevelType::ELT_InGame:
		return GetRandomLevelName();
	case ELevelType::ELT_Result:
		return ResultLevelName;
	}

	return FString();
}
void UInGameInstance::InitTotalPlayerCount()
{
	TotalPlayerCount = 0;
}
void UInGameInstance::AddTotalPlayerCount()
{
	++TotalPlayerCount;
}
