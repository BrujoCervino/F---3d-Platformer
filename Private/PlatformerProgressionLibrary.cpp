// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerProgressionLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlatformerLevelScriptActor.h"

bool UPlatformerProgressionLibrary::RequestLevelComplete(UObject* const WorldContext)
{
	bool bSuccessful = false;
	
	if (WorldContext)
	{
		if (UWorld* const CurrentWorld = WorldContext->GetWorld())
		{
			// Get the level script actor, cast to a platformer level script
			APlatformerLevelScriptActor* const CurrentLevelScript = 
				Cast<APlatformerLevelScriptActor, ALevelScriptActor>(CurrentWorld->GetLevelScriptActor());
			
			if (CurrentLevelScript)
			{
				CurrentLevelScript->CompleteLevel();
				bSuccessful = true;
			}
		}
	}

	return bSuccessful;
}