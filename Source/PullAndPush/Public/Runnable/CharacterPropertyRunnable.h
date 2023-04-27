#pragma once

#include "PullAndPush.h"
#include "HAL/Runnable.h"

DECLARE_DELEGATE_OneParam(FBUIOnCompleteSignature, const bool&)

class PULLANDPUSH_API FCharacterPropertyRunnable : public FRunnable
{
public:
	FCharacterPropertyRunnable();
	virtual ~FCharacterPropertyRunnable();

	// FRunnable functions
	virtual bool Init() override; 
	virtual uint32 Run() override;
	virtual void Stop() override;

	// Set&Get Character Properties..
	void SetMoveSpeed(float NewSpeed);

	UFUNCTION()
	FORCEINLINE float GetMoveSpeed() { return CurrentMoveSpeed.load(); }

	FBUIOnCompleteSignature CompleteSignature;

private:
	FRunnableThread* Thread;
	uint8 bRunThread : 1;
	uint8 bInputReady : 1;

	// Character Properties..
	std::atomic<float> CurrentMoveSpeed;
	std::atomic<float> PendingMoveSpeed;
};