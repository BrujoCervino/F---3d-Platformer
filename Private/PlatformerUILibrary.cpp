// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerUILibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

bool UPlatformerUILibrary::ToggleForceFeedbackEnabled(UObject* WorldContext)
{
	bool bForceFeedbackBecameEnabled = false;

	if (UWorld* const World = WorldContext->GetWorld())
	{
		// Get all player controllers
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (APlayerController* const PC = Iterator->Get())
			{
				// Since the vibration will be the same across all player controllers, 
				//	get the result of the first one and store its toggled result.
				if (0 == Iterator.GetIndex())
				{
					bForceFeedbackBecameEnabled = !(PC->bForceFeedbackEnabled);
				}
				// Use our stored result across all player controllers.
				PC->bForceFeedbackEnabled = bForceFeedbackBecameEnabled;
			}
		}
	}

	return bForceFeedbackBecameEnabled;
}

bool UPlatformerUILibrary::SetForceFeedbackEnabled(UObject* WorldContext, const bool bEnabled)
{
	bool bForceFeedbackBecameEnabled = false;

	if (UWorld* const World = WorldContext->GetWorld())
	{
		// Get all player controllers
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (APlayerController* const PC = Iterator->Get())
			{
				// 
				PC->bForceFeedbackEnabled = bEnabled;
			}
		}
		bForceFeedbackBecameEnabled = true;
	}

	return bForceFeedbackBecameEnabled;
}
