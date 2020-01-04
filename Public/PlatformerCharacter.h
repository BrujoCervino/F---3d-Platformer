// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlatformerCharacter.generated.h"

class USoundCue;

UCLASS()
class F_API APlatformerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlatformerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Called for forwards/backward input
	void MoveForward(float Value);

	// Called for side to side input
	void MoveRight(float Value);

	// "Called when the actor falls out of the world 'safely' (below KillZ and such)"
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;
	
	// Exposes FellOutOfWorld delegate to Blueprints, for rapid prototyping.
	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected))
	void OnFellOutOfWorld();	// Defined and called from within Blueprints

	// Called upon landing when falling, to perform actions based on the Hit result. Triggers the OnLanded event.
	virtual void Landed(const FHitResult& Hit) override;

	// Turns the camera to face where the player is looking.
	// Useful for players who aren't used to turning the right stick
	virtual void FacePlayerDirection();	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//		Abilities: Air Dash
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Reflection: First I tried to use ACharacter::AddMovementInput, but that is made for running not jumping

	// Propell the character forwards (character- or camera-relative)
	virtual void AirDash();

	// Getter and setter for bCanAirDash
	inline bool CanAirDash() const { return bCanAirDash; }
	void SetCanAirDash(const bool bNewCanAirDash);

private:

	// The strength of the force applied to the player when dashing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Air Dash", meta = (AllowPrivateAccess = "true"))
	float DashSpeed;

	// Whether this character can currently air dash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities|Air Dash", meta = (AllowPrivateAccess = "true"))
	uint32 bCanAirDash : 1;

	// The sound played on air dashing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Air Dash", meta = (AllowPrivateAccess = "true"))
	USoundCue* AirDashSound;

	// (For accessibility/player preference): whether air dash is executed the direction the camera is facing 
	// or in the direction the character is facing 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Air Dash", meta = (AllowPrivateAccess = "true"))
	uint32 bAirDashIsCameraRelative : 1;

protected:

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//		Abilities: Shrink
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Toggle this character between a tiny version of itself, able to reach small spaces, and the standard version
	virtual void ToggleShrink();

	UFUNCTION(BlueprintImplementableEvent, Category = "Abilities|Shrink", meta = (BlueprintProtected))
	void OnShrinkToggled();

	// Getter and setter for bCanShrink
	inline bool CanShrink() const { return bCanShrink; }
	UFUNCTION()
	void SetCanShrink(const bool bNewCanShrink);

	// Whether the player is currently shrunk. 
	// (Maybe it needs its own flag aside from just checking the scale?)
	bool IsShrunk() const;

private:

	// Whether this character is allowed to shrink at the current time
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="Abilities|Shrink", meta=(AllowPrivateAccess="true"))
	uint32 bCanShrink : 1;

	// The sound played when shrinking
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Shrink", meta = (AllowPrivateAccess = "true"))
	USoundCue* ShrinkSound;
	
	// The sound played when returning to normal size
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Shrink", meta = (AllowPrivateAccess = "true"))
	USoundCue* SizeUpSound;

	// Scale of this actor when shrunk
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Shrink", meta = (AllowPrivateAccess = "true"))
	float ShrunkScale;

	// Scale of this actor when shrink is inactive
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Shrink", meta = (AllowPrivateAccess = "true"))
	float StandardScale;

	// The time dilation to apply when shrunk
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Shrink", meta = (AllowPrivateAccess = "true"))
	float ShrunkTimeDilation;

	// The time dilation to apply when shrink is inactive
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Shrink", meta = (AllowPrivateAccess = "true"))
	float StandardTimeDilation;

	// How long this player has to wait until able to shrink again (prevents cheating/spamming)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Shrink", meta = (AllowPrivateAccess = "true"))
	float ShrinkCooldown;

	// The timer handle for the shrink cooldown
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Shrink", meta = (AllowPrivateAccess = "true"))
	FTimerHandle ShrinkCooldownHandle;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//		Abilities: Interact
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

protected:

	// Check if an interactable (implementor of the interactable interface) is nearby, interacting with it if so.
	virtual void TraceForInteractables();
	UFUNCTION(BlueprintImplementableEvent, Category = "Abilities|Interact", meta = (BlueprintProtected))
	void OnInteracted();

	// Getter and Setter for bCanInteract
	inline bool CanInteract() const { return bCanInteract; }
	UFUNCTION()
	void SetCanInteract(const bool bNewCanInteract);
	
private:

	// Whether this character is allowed to interact at the current time
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	uint32 bCanInteract : 1;

	// Length of the raycast for interaction
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	float InteractionTraceLength;

	// 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	float InteractionTraceCapsuleRadius; 
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	float InteractionTraceCapsuleHalfHeight;

	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> InteractionTraceDesiredTypes;

	// Whether to trace for simple or complex geometry when tracing for interactables
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	uint32 bInteractionTraceComplex : 1;

	// Actors (excluding this actor) to exclude when tracing for interactables
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true")) 
	TArray<AActor*> InteractionTraceActorsToIgnore;

	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	uint32 bDrawInteractionTrace : 1;

	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	USoundCue* InteractionSucceededCue;

	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	USoundCue* InteractionFailedCue;

	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	float InteractionCooldown;

	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	FTimerHandle InteractionCooldownHandle;

};