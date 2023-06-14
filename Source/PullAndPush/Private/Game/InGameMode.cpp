#include "Game/InGameMode.h"
#include "Game/InGameInstance.h"
#include "Kismet/GameplayStatics.h"

AInGameMode::AInGameMode()
	:
	TotalPlayerCount(2), CurrentPlayerCount(0), MaxRoundCount(2), MaxLevelCount(0)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
}
void AInGameMode::StartPlay()
{
	Super::StartPlay();

	// Set GameInstance Value
	InGameInstance = Cast<UInGameInstance>(GetGameInstance());
	if (InGameInstance)
	{
		if (!InGameInstance->IsFirstRoundStart())
		{
			InGameInstance->InitSetting(MaxRoundCount);
		}
	}
}
void AInGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// @TODO : ������ ĳ���� Stat ������ �ʱ�ȭ
	

	/** Round start when all players enter */
	if (++CurrentPlayerCount >= TotalPlayerCount)
	{
		RoundStart();
	}
}
void AInGameMode::PlayerFellOutOfWorld()
{
	PPLOG(Warning, TEXT("Player Fell Out Of World!"));
	if (--CurrentPlayerCount <= 1)
	{
		RoundEnd();
	}
}
void AInGameMode::RoundStart()
{
	PPLOG(Warning, TEXT("RoundStart!"));

	// @TODO : ������ �Ѹ��� 3,2,1 ���� ����!
}
void AInGameMode::RoundEnd()
{
	PPLOG(Warning, TEXT("RoundEnd"));

	// Access GameInstance and Switch Level
	if(!InGameInstance->IsAllRoundsFinished())
	{
		SwitchToNextLevel();
	}
	else
	{
		AllRoundsCompleted();
	}
}
void AInGameMode::SwitchToNextLevel()
{
	FString NextLevelName = GetRandomLevelName();
	GetWorld()->ServerTravel(NextLevelName);
}
void AInGameMode::AllRoundsCompleted()
{
	GetWorld()->ServerTravel(ResultLevelName);
}
FString AInGameMode::GetRandomLevelName()
{
	int LevelIdx = FMath::RandRange(1, MaxLevelCount);
	FString LevelName = LevelDirectoryPath + FString::FromInt(LevelIdx);
	return LevelName;
}