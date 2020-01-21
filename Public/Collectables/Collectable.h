// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformerTypes.h"
#include "Collectable.generated.h"

class USoundCue;

UCLASS(Blueprintable)
class F_API ACollectable
	: public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACollectable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called when this collectable is picked up by a collector
	UFUNCTION(BlueprintCallable)
	virtual void OnCollected(AActor* Collector);
	UFUNCTION(BlueprintImplementableEvent, meta = (Displayname = "OnCollected")) // Blueprint implementation
	void ReceiveOnCollected(AActor* Collector);

	// Returns bCollected
	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline bool HasBeenCollected() const { return bCollected; }

	// Returns mesh
	inline UStaticMeshComponent* GetMesh() const { return Mesh; }

protected:

	// The type associated with this collectable
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collectable", meta = (BlueprintProtected = "true"))
	TEnumAsByte<ECollectableTypes::Type> CollectableType;



private:

	// The default mesh associated with this collectable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectable", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	// The sound to play when this collectable has been picked up
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collectable", meta = (AllowPrivateAccess = "true"))
	USoundCue* CollectedSound;

	// Whether this collectable has been collected.
	UPROPERTY(EditDefaultsOnly, Category = "Collectable", meta = (AllowPrivateAccess = "true"))
	uint32 bCollected : 1;

};
