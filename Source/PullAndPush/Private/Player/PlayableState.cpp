#include "Player/PlayableState.h"
#include "Net/UnrealNetwork.h"

void APlayableState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

    if (APlayableState* NewPlayerState = Cast<APlayableState>(PlayerState))
    {
        NewPlayerState->RankScore = RankScore;
    }
}
void APlayableState::Reset()
{
	Super::Reset();

	bIsDead = 0;
}
void APlayableState::SetRankScore(const int8 NewRankScore)
{
	RankScore = NewRankScore;
}
void APlayableState::SetIsDead(const bool NewState)
{
	bIsDead = NewState;
}
void APlayableState::OnRep_RankScore()
{
}
void APlayableState::OnRep_IsPlayerDead()
{
}
void APlayableState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(APlayableState, RankScore, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(APlayableState, bIsDead, SharedParams);
}