// Fill out your copyright notice in the Description page of Project Settings.


#include "ShrinkBanVolume.h"
#include "GameFramework/PlatformerCharacter.h"

void AShrinkBanVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (APlatformerCharacter* const PlayerCharacter = Cast<APlatformerCharacter, AActor>(OtherActor))
	{
		const bool bShouldPlayUnshrinkingSound = PlayerCharacter->IsShrunk();
		
		// Unshrink the player
		PlayerCharacter->EnsureStandardSize(bShouldPlayUnshrinkingSound);
		
		// Ban shrinking
		PlayerCharacter->SetCanShrink(false);
	}
}

void AShrinkBanVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (APlatformerCharacter* const PlayerCharacter = Cast<APlatformerCharacter, AActor>(OtherActor))
	{
		// Rid the ban on shrinking
		PlayerCharacter->SetCanShrink(true);
	}
}
