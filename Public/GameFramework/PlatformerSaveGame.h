// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlatformerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class F_API UPlatformerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Abilities")
	uint32 bShrinkUnlocked : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities")
	int32 SkillPointsEarnt;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities")
	int32 SkillPointsSpent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities")
	int32 Keys;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	int32 SecretKeys;
};
