// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlatformerUILibrary.generated.h"

/**
 * Exists to avoid duplicate code for UI functionalitites
 */
UCLASS()
class F_API UPlatformerUILibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Toggles whether force feedback is enabled for ALL valid player controllers.
	// Limitation: doesn't save the data for future play sessions yet
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static bool ToggleForceFeedbackEnabled(UObject* WorldContext);

private:

	// Sets whether force feedback is enabled for ALL valid player controllers.
	// Limitation: doesn't save the data for future play sessions yet
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static bool SetForceFeedbackEnabled(UObject* WorldContext, const bool bForceFeedbackEnabled);
};
