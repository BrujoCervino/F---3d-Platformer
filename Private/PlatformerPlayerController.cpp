// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerPlayerController.h"
#include "Kismet/GameplayStatics.h"

APlatformerPlayerController::APlatformerPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true; // Needs to tick, otherwise input does not work.
}

void APlatformerPlayerController::BeginPlay()
{
	Super::BeginPlay();
}


