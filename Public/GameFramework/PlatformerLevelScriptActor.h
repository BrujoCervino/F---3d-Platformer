// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "PlatformerLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class F_API APlatformerLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:

	// Should be called when the player has finished this level:
	// TODOs:
	// * (MAYBE) Show the player their completion rate & Time taken
	// * (DONE) Dish out any rewards for completing this level
	// * (DONE) Teleport the player to the next level
	UFUNCTION(BlueprintCallable)
	virtual void CompleteLevel();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="CompleteLevel"))
	void OnLevelCompleted();

};
