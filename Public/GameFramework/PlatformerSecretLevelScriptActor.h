// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlatformerLevelScriptActor.h"
#include "PlatformerSecretLevelScriptActor.generated.h"

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
