// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlatformerLevelScriptActor.h"
#include "PlatformerSecretLevelScriptActor.generated.h"

// TODO: Reward the player with a skill point for finishing the secret level
// TODO: Open the level which contained the portal to the secret level
// TODO: Ensure there are no GetPlayerController instances: I want things player-specific

/**
 * 
 */
UCLASS()
class F_API APlatformerSecretLevelScriptActor : public APlatformerLevelScriptActor
{
	GENERATED_BODY()
	
public:

	APlatformerSecretLevelScriptActor();

	// See APlatformerLevelScriptActor::CompleteLevel
	virtual void CompleteLevel() override;

	UPROPERTY(EditDefaultsOnly)
	FName LevelToOpen;

};
