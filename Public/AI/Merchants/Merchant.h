// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Merchant.generated.h"

// A merchant is a shopkeeper from whom the player can buy items/abilities.
UCLASS()
class F_API AMerchant 
	: public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMerchant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//~ Begin Interface IInteract
	virtual void Interact(AActor* Interactor) override;
	//~ End Interface IInteract

	// Returns bTalking
	inline bool IsTalking() const { return bTalking; }

private:

	// Whether this merchant is currently engaged in a conversation (whether they have to talk to the player).
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Conversation", meta = (AllowPrivateAccess="true"))
	uint32 bTalking : 1;
};
