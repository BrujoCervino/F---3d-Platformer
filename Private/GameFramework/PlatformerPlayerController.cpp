// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformerPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlatformerSaveGame.h"

APlatformerPlayerController::APlatformerPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true; // Needs to tick, otherwise input does not work.

	LoadFromSaveData();
}

void APlatformerPlayerController::GrantSkillPoint()
{
	++SkillPointsEarnt;
}

void APlatformerPlayerController::ConsumeSkillPoints(const int32 Cost)
{
	const int32 NumSkillPointsAfterSpend = GetSkillPointsEarntPostDeductions() - Cost;
	// If this transaction does not bring the num skill points (postdeduction) below 0,
	if (NumSkillPointsAfterSpend >= 0)
	{
		// Add the cost to the number of skill points spent
		SkillPointsSpent += Cost;
	}
}

bool APlatformerPlayerController::UseKey()
{
	// If this player has any keys,
	const bool bUsedAKey = (Keys > 0);
	if (bUsedAKey)
	{
		// Use a key.
		--Keys;
	}

	return bUsedAKey;
}

void APlatformerPlayerController::GiveKey()
{
	++Keys;
}

bool APlatformerPlayerController::UseSecretKey()
{
	// If this player has any keys,
	const bool bUsedAKey = (SecretKeys > 0);
	if (bUsedAKey)
	{
		// Use a key.
		--SecretKeys;
	}

	return bUsedAKey;
}

void APlatformerPlayerController::GiveSecretKey()
{
	++SecretKeys;
}

void APlatformerPlayerController::LoadFromSaveData()
{
	bShrinkUnlocked = false;
	SkillPointsEarnt = 0;
	SkillPointsSpent = 0;
	Keys = 0;
	SecretKeys = 0;

	FString const SaveSlotName = TEXT("Player1");
	int32 const SaveSlotIndex = 0;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveSlotIndex))
	{
		UPlatformerSaveGame const * const SaveGame = 
			Cast<UPlatformerSaveGame, USaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveSlotIndex)); 

		if (SaveGame)
		{
			bShrinkUnlocked	 = SaveGame->bShrinkUnlocked;
			SkillPointsEarnt = SaveGame->SkillPointsEarnt;
			SkillPointsSpent = SaveGame->SkillPointsSpent;
			Keys			 = SaveGame->Keys;
			SecretKeys		 = SaveGame->SecretKeys;
			Orbs			 = SaveGame->Orbs;
			Flies			 = SaveGame->Flies;
			Cells			 = SaveGame->Flies;
		}
	}

}


