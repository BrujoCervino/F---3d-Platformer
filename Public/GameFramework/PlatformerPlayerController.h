// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlatformerPlayerController.generated.h"

// TODO: Finish this struct type for each collectable type within the player controller 
// (this would solve lots of duplicate code). Something like this:
//USTRUCT()
//template<typename TNumericType>
//struct FCollectableData
//{
//	GENERATED_USTRUCT_BODY()
//	
//	FCollectableData<TNumericType>() 
//	{
//	}
//	
//	// Spend the given number of this collectable
//	bool Spend(const TNumericType Cost) { checkNoEntry(); }
//
//	// Returns the total number of this collectable type which the player has earnt across all levels
//	TNumericType GetEarntTotal() const {  return TNumericType(); }
//
//	// Returns the total number of this collectable type which the player has earnt in the specified level
//	TNumericType GetEarntInLevel(const int LevelIndex) const { TNumericType(); }
//
//	// Returns the number of this collectable type the player has spent across the whole game
//	TNumericType GetSpent() const { TNumericType(); }
//
//	// Returns GetEarntTotal() - GetSpent()
//	TNumericType GetEarntTotalPostDeductions() const { TNumericType(); }
//
//private:
//
//	TNumericType NumSpent;
//
//	TArray<TNumericType> NumEarntInEachLevel;
//	
//	// Ban the typeless constructor.
//	FCollectableData() {} 
//};


/**
 * The default player controller for this game.
 * 
 *
 */
UCLASS()
class F_API APlatformerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APlatformerPlayerController();

	// Getter for bShrinkUnlocked
	inline bool IsShrinkUnlocked() const { return bShrinkUnlocked; }

	//
	inline int32 GetSkillPointsEarnt() const { return SkillPointsEarnt; }
	inline int32 GetSkillPointsSpent() const { return SkillPointsSpent; }					  
	inline int32 GetSkillPointsEarntPostDeductions() const { return SkillPointsEarnt - SkillPointsSpent; }

	// Give this player one more skill point
	UFUNCTION(BlueprintCallable)
	virtual void GrantSkillPoint();

	// Spend a specified amount of skill points 
	UFUNCTION(BlueprintCallable)
	virtual void ConsumeSkillPoints(const int32 Cost);

	// If this character has enough keys, use one.
	// Returns whether a key was used.
	UFUNCTION(BlueprintCallable)
	virtual bool UseKey();

	// Increments keys
	UFUNCTION(BlueprintCallable)
	virtual void GiveKey();

	// If this character has enough secret keys, use one.
	// Returns whether a key was used.
	UFUNCTION(BlueprintCallable)
	virtual bool UseSecretKey();

	// Increments secret keys
	UFUNCTION(BlueprintCallable)
	virtual void GiveSecretKey();

protected:

	// Load game data, via a save file
	void LoadFromSaveData();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Shrink is an ability used by the player to reach hidden areas
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	// Whether the player has unlocked the shrink ability yet
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	uint32 bShrinkUnlocked : 1;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Skill points, gained by completing secret missions, are used to buy/upgrade new moves (shrink et cetera)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	// The number of skill points this player has earnt
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	int32 SkillPointsEarnt;

	// The number of skill points this player has spent
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	int32 SkillPointsSpent;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Keys are used to unlock doors
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	// The number of keys held by the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	int32 Keys;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Secret keys are used to unlock doors to secret levels
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	// The number of secret keys held by the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	int32 SecretKeys;


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Orbs, the most common collectables, are spent on TODO
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	int32 Orbs;


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 7 Flies can be found in each level. Find them all grants the player a cell.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	int32 Flies;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Cells are collected by completing tasks in each level. Collecting enough grants the player access to new levels.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	int32 Cells;
};
