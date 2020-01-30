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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collectables")
	int32 SkillPointsEarnt;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collectables")
	int32 SkillPointsSpent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collectables")
	int32 Keys;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collectables")
	int32 SecretKeys;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collectables")
	int32 Orbs;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collectables")
	int32 Flies;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collectables")
	int32 Cells;

};
