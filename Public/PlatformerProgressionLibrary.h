// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlatformerProgressionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class F_API UPlatformerProgressionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	// Requests completion of the current level (see PlatformerLevelScriptActor.h)
	// Returns whether the level sucessfully completed.
	// Limitation: Doesn't consider level streaming yet.
	UFUNCTION(BlueprintCallable)
	static bool RequestLevelComplete(UObject* const WorldContext);

};
