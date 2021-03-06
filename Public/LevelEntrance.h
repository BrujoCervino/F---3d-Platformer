// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "LevelEntrance.generated.h"

UCLASS()
class F_API ALevelEntrance
	: public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelEntrance();

	//~ Begin IInteractable Interface
	virtual void Interact(AActor* Interactor) override;
	//~ End IInteractable Interface

	// Returns the shared root
	USceneComponent* GetSharedRoot() const { return SharedRoot; }

	// Returns left mesh
	UStaticMeshComponent* GetLeftDoorMesh() const { return LeftDoorMesh; }

	// Returns right mesh
	UStaticMeshComponent* GetRightDoorMesh() const { return RightDoorMesh; }

	// Returns LevelToOpen
	FName GetLevelToOpen() const { return LevelToOpen; }

	// Sets bLocked
	void SetLocked(const bool bNewLocked);
	// Returns bLocked
	UFUNCTION(BlueprintCallable)
	bool IsLocked() const { return bLocked; }

private:

	// The pivot of this entrance: without it, the doors would move (undesirably) relatively to each other.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SharedRoot;

	// The mesh associated with the left door of this entrance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeftDoorMesh;

	// The mesh associated with the right door of this entrance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightDoorMesh;

	// The level to which this entrance sends the player	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName LevelToOpen;

	// Whether this door is locked.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint32 bLocked : 1;

};