// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlatformerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class F_API APlatformerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	// Should be called when the player has reached the end of the game: 
	// TODOs: 
	// * Roll the credits
	// * Dish out rewards (new game+, new costumes et cetera)
	// * Take the player back to the main menu
	UFUNCTION(BlueprintCallable)
	virtual void CompleteGame();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CompleteGame"))
	void OnGameCompleted();
};
