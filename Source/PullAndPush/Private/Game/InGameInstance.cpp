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
}
void UInGameInstance::TravelLevel(ELevelType LevelType)
{
	if (LevelType != ELevelType::ELT_Main)
	{
		GetWorld()->ServerTravel(GetLevelPathOfEnum(LevelType));
	}
	else
	{
		ResetData();
		UGameplayStatics::OpenLevel(this, MainLevelName);
	}
}
FString UInGameInstance::GetRandomLevelName()
{
	// Get Random Level used by AssetManager
	UAssetManager& Manager = UAssetManager::Get();
	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("Map"), Assets);
	ensure(0 < Assets.Num());

	FString AssetName;
	if (Assets.Num() >= 1)
	{
		int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
		FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
		AssetName = LevelDirectoryPath + AssetPtr.GetAssetName();
	}

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