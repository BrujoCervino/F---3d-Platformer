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
	UGameplayStatics::OpenLevel(this, LevelToOpen);
}