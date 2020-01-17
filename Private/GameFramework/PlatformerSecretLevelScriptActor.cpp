// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerSecretLevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlatformerPlayerController.h"

APlatformerSecretLevelScriptActor::APlatformerSecretLevelScriptActor()
	:
	LevelToOpen()
{

}

void APlatformerSecretLevelScriptActor::CompleteLevel()
{
	// Give the player one skill point for having completed this secret level
	APlatformerPlayerController* const PC = Cast<APlatformerPlayerController, APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->GrantSkillPoint();
	}

	UGameplayStatics::OpenLevel(this, LevelToOpen);
}