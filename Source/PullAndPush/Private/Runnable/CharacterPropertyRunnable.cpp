#include "Runnable/CharacterPropertyRunnable.h"

FCharacterPropertyRunnable::FCharacterPropertyRunnable()
	:
	Thread(nullptr), bRunThread(true), bInputReady(false)
{
	Thread = FRunnableThread::Create(this, TEXT("Character Properties Thread"));
}
FCharacterPropertyRunnable::~FCharacterPropertyRunnable()
{
	if (Thread != nullptr)
	{
		Thread->Kill();
		delete Thread;
	}
}
bool FCharacterPropertyRunnable::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Character Properies Thread Init!"));
	return true;
}
uint32 FCharacterPropertyRunnable::Run()
{
	while (bRunThread)
	{
		if (bInputReady)
		{
			// Changing Speed if Diff
			float NewSpeed = PendingMoveSpeed.exchange(CurrentMoveSpeed);
			if (NewSpeed != CurrentMoveSpeed) {
				CurrentMoveSpeed = NewSpeed;
				bInputReady = false;
				CompleteSignature.ExecuteIfBound(false);	// Delegate to Character

				UE_LOG(LogTemp, Warning, TEXT("[CharacterPropertyRunnable] Change Character Speed %f"), NewSpeed);
			}
			FPlatformProcess::Sleep(1.0f); 
		}
	}
	return 0;
}
void FCharacterPropertyRunnable::Stop()
{
	bRunThread = false;
}
void FCharacterPropertyRunnable::SetMoveSpeed(float NewSpeed)
{
	PendingMoveSpeed.store(NewSpeed);
	bInputReady = true;
}
