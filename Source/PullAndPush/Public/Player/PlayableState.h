// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayableState.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API APlayableState : public APlayerState
{
	GENERATED_BODY()

protected:
    APlayableState() : RankScore(0), bIsDead(0){}

    virtual void CopyProperties(APlayerState* PlayerState);

public:
    virtual void Reset() override;

// Custom Value
protected:
    /** Player's Rank count */
    UPROPERTY(ReplicatedUsing = OnRep_RankScore)
    int8 RankScore;

    /** Player's Dead */
    UPROPERTY(ReplicatedUsing = OnRep_IsPlayerDead)
    uint8 bIsDead:1;

public:
    UFUNCTION()
    void SetRankScore(const int8 NewRankScore);

    UFUNCTION()
    FORCEINLINE int8 GetRankScore() {return RankScore;}

    UFUNCTION()
    void OnRep_RankScore();

    UFUNCTION()
    void SetIsDead(const bool NewState);

    UFUNCTION()
    FORCEINLINE bool GetIsDead() { return bIsDead; }

    UFUNCTION()
    void OnRep_IsPlayerDead();
};
