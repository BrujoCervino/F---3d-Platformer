// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelEndCrystal.generated.h"

class UInterpToMovementComponent;
class URotatingMovementComponent;

UCLASS()
class F_API ALevelEndCrystal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelEndCrystal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Returns mesh
	inline UStaticMeshComponent* GetMesh() const { return Mesh; }

	// Returns bCollected
	inline bool HasBeenCollected() const { return bCollected; }

	// Event when this actor overlaps another actor, for example a player walking into a trigger.
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:

	// The main mesh associated with this crystal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	// The component responsible for oscillating this crystal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInterpToMovementComponent* InterpComp;

	// The rate (degrees) at which to rotate this mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float YawRotationRate;

	// Whether this crystal has been collected. Prevents spamming
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint32 bCollected : 1;
};
