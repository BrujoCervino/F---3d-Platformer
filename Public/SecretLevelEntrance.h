// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelEntrance.h"
#include "SecretLevelEntrance.generated.h"

UCLASS()
class F_API ASecretLevelEntrance : public ALevelEntrance
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecretLevelEntrance();

	//~ Begin IInteractable Interface
	virtual void Interact(AActor* Interactor) override;
	//~ End IInteractable Interface
};
