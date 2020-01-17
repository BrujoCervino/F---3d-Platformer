// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlatformerCharacter.generated.h"

// WARNING/TODO: THE CONFIG VARIABLES NEED TO BE SETTABLE FROM WITHIN THE UI.
// TODO: look into events within Misc/CoreDelegates.h : lots of goodies in there.
// TODO: Secondary air dash should only be executable within a few seconds of shrinking down. If unused, set used to true. 
// This makes the ability properly advanced and chainable
// TODO: ResetSceneFringeIntensity should lerp in a V shape: lerp to DamagedIntensity for about 20% of the time, StandardIntensity for 80% of the time
/* TODO: revamp interaction trace system: 
* Should be toggleable between camera- and character relative 
* Probably use a capsule component attachable to camera or mesh (avoids mess within Tick)
* Plan: onOverlapBegin: display a widget indicating player can interact and store a pointer to the currently interactable actor;
*		onOverlapEnd, hide the widget and nullify the pointer
*/

class UCameraComponent;
class UCameraShake;
class UForceFeedbackEffect;
class UMaterialParameterCollection;
class UMaterialParameterCollectionInstance;
class USoundCue;
class USpringArmComponent;

/**
* The main playable character of this game.
* Is third person and can jump, dash, stomp and interact (which can be chained)
*/
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

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//		Basics: Movement & Camera
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:

	// Returns CameraBoom subobject
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// Returns FollowCamera subobject
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

protected:

	// Called for forwards/backward input
	void MoveForward(float Value);

	// Called for side to side input
	void MoveRight(float Value);

	// Called via input to turn at a given rate.
	// Rate: This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	void TurnAtRate(float Rate);

	// Called via input to look up/down at a given rate.
	// Rate: This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	void LookUpAtRate(float Rate);

	// "Called when the actor falls out of the world 'safely' (below KillZ and such)"
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;
	
	// Exposes FellOutOfWorld delegate to Blueprints, for rapid prototyping.
	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected))
	void OnFellOutOfWorld();	// Defined and called from within Blueprints

	// Called upon landing when falling, to perform actions based on the Hit result. Triggers the OnLanded event.
	virtual void Landed(const FHitResult& Hit) override;

	// Turns the camera to face where the player is looking.
	// Useful for players who aren't used to turning the right stick (those who don't play 3d games)
	virtual void FacePlayerDirection();	

private:

	// Camera boom positioning the camera behind the character 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// (For accessibility/player preference): Allows the game to be played with one hand, 
	// by locking the camera to where the mesh is facing.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	uint32 bUseOneHandedCamera : 1;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//		Basics: Damage
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	
	// Apply damage to this actor (see AActor::TakeDamage)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Kills this player then calls Respawn
	UFUNCTION(BlueprintCallable, Category = "Damage", meta=(BlueprintProtected))
	virtual void Die(AActor* Killer);
	// Expose the Die event to Blueprint scripting
	UFUNCTION(BlueprintImplementableEvent, Category = "Damage", meta = (BlueprintProtected))
	void OnDied(AActor* Killer);

	// Register a checkpoint, for future respawns
	UFUNCTION(BlueprintCallable, Category = "Damage", meta = (BlueprintProtected))
	void RegisterCheckpoint(AActor const * const CheckpointActor);

	// Returns bInvincible
	bool IsInvincible() const { return bInvincible; }

	// Returns MaxHealth
	float GetMaxHealth() const { return MaxHealth; }
	// Returns Health
	float GetHealth() const { return Health; }

protected:

	// Plays the death transition, then respawns
	UFUNCTION(Category = "Damage")
	virtual void Prerespawn();

	// Called upon death: restarts the player from the most recent checkpoint
	virtual void Respawn();

	// Sets bInvincible
	UFUNCTION(Category = "Damage")
	void SetInvincible(const bool bNewInvincible);

	// Interpolates FollowCamera->PostProcessSettings.SceneFringeIntensity from DamagedSceneFringeIntensity
	//	to StandardSceneFringeIntensity
	UFUNCTION(Category = "Damage")
	void ResetSceneFringe();

private:

	// Whether this character can take damage from hazards or enemies (different to the more general AActor::bCanBeDamaged)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true")) 
	uint32 bInvincible : 1;

	// How long to apply invincibility when this character has just taken damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true")) 
	float InvincibilityDuration;

	// The handle for temporary invincibility (after taking damage from hazards or enemies)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true")) 
	FTimerHandle InvincibilityTimerHandle;

	// The health of this character when health is fully restored.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true")) 
	float MaxHealth;

	// The current health of this character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true")) 
	float Health;

	// The amount this character will be launched by when damaged
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamagedLaunchStrength;

	// The colour this character will turn when damaged
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true")) 
	FLinearColor DamagedColour;

	// The camera shake to play when damaged
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShake> DamagedCameraShake;

	// The sound to play when damaged
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	USoundCue* DamagedSound;

	// The vibration to play when damaged
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	UForceFeedbackEffect* DamagedVibration;

	// The scene fringe (chromatic abberation) intensity applied during normal gameplay
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true")) 
	float StandardSceneFringeIntensity;

	// The scene fringe (chromatic abberation) intensity applied when damaged
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true")) 
	float DamagedSceneFringeIntensity;

	// How long the scene fringe will take to fade back to normal
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true")) 
	float SceneFringeResetDuration;

	// The timer handle for resetting chromatic aberration after taking damage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true")) 
	FTimerHandle SceneFringeResetTimerHandle;

	// The location of the last encountered checkpoint. This character will respawn here after death
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	FVector MostRecentCheckpointLocation;

	// The sound played when registering a checkpoint
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	USoundCue* CheckpointRegisteredCue;

	// The colour this player will glow when registering a checkpoint
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	FLinearColor CheckpointRegisteredColour;

	// The name of the scalar parameter used for the radius of the death transition
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	FName DeathTransitionRadiusName;

	// 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DeathTransitionRadiusProgress;

	// The material parameter collection type used with the death transition
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	UMaterialParameterCollection* DeathTransitionMaterialParamCollectionType;

	// 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	UMaterialParameterCollectionInstance* DeathTransitionMaterialParamCollectionInst;

	// 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DeathTransitionTimerFrequency;

	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true")) 
	FTimerHandle DeathTransitionTimerHandle;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//		Basics: Collectables
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

public:

	// Uses a key if one is available (masks APlatformerPlayerController::UseKey).
	UFUNCTION(BlueprintCallable)
	bool UseKey();

	// Gives a key to this character (masks APlatformerPlayerController::GiveKey).
	UFUNCTION(BlueprintCallable)
	bool GiveKey();

private:

	// The colour this player will glow when collecting a key.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectables", meta = (AllowPrivateAccess = "true"))
	FLinearColor KeyCollectedColour;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//		Game Feel
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

public:

	// Applies a temporary radial glow to the main mesh's material
	// (To be used when collecting a pickup or taking damage) 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "GameFeel", meta = (BlueprintProtected))
	void ApplyRadialColour(const FLinearColor& GlowColour);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//		Abilities: Air Dash
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

protected:

	// Propell the character forwards (character- or camera-relative)
	virtual void AirDash();

	// Accessors for bCanAirDash
	inline bool CanAirDash() const { return bCanAirDash; }
	void SetCanAirDash(const bool bNewCanAirDash);

private:

	// The strength of the force applied to the player when dashing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Air Dash", meta = (AllowPrivateAccess = "true"))
	float DashSpeed;

	// Whether this character can currently air dash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities|Air Dash", meta = (AllowPrivateAccess = "true"))
	uint32 bCanAirDash : 1;

	// (Advanced tactic) Whether this character can perform a second air dash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities|Air Dash", meta = (AllowPrivateAccess = "true"))
	uint32 bCanAirDashSecondary : 1;

	// Whether this character has used its primary air dash in the current instance of being in the air.
	// This flag exists so I can ensure the player can only (dash, shrink, dash) as an advanced technique,
	// rather than the valueless (shrink, dash, dash)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities|Air Dash", meta = (AllowPrivateAccess = "true"))
	uint32 bHasUsedAirDashPrimary : 1;

	// Whether this character has used its secondary air dash in the current instance of being in the air
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities|Air Dash", meta = (AllowPrivateAccess = "true"))
	uint32 bHasUsedAirDashSecondary : 1;

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
	// Notify the Blueprint this player changed size, so it can add prototypical features.
	UFUNCTION(BlueprintImplementableEvent, Category = "Abilities|Shrink", meta = (BlueprintProtected))
	void OnShrinkToggled();

	// Accessors for bCanShrink
	inline bool CanShrink() const { return bCanShrink; }
	UFUNCTION()
	void SetCanShrink(const bool bNewCanShrink);

	// Whether the player is currently shrunk. 
	// (Maybe it needs its own flag aside from just checking the scale?)
	bool IsShrunk() const;

private:

	// Whether this character is allowed to shrink at the current time
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Abilities|Shrink", meta = (AllowPrivateAccess = "true"))
	uint32 bCanShrink : 1;

	// The camera shake to play when the player toggles shrink 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Shrink", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShake> ShrinkToggledCameraShake;

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

	// Whether to draw the capsule trace used for interacting
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	uint32 bDrawInteractionTrace : 1;

	// The sound to play when this player successfully interacts with something
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	USoundCue* InteractionSucceededCue;

	// The sound to play when this player fails to interact with something (detected an object but it's not interactable).
	// Not sure whether this will stay in the game: though it makes the game feel more responsive, it can be annohyingly spammed.
	// A better solution would be to display a symbol on-screen when an interactable is in view
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	USoundCue* InteractionFailedCue;

	// The time (seconds), after interaction, to wait until reenabling it.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	float InteractionCooldown;

	// 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Interact", meta = (AllowPrivateAccess = "true"))
	FTimerHandle InteractionCooldownHandle;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//		Abilities: Stomp
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

protected:

	// Propell the character downwards, as though weighed down
	virtual void Stomp();

private:

	// The gravity scale to apply to this character when stomping
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Stomp", meta = (AllowPrivateAccess = "true"))
	float StompingGravityScale;

	// The gravity scale to apply to this character when not stomping
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Stomp", meta = (AllowPrivateAccess = "true"))
	float StandardGravityScale;

	// The camera shake to play when this player lands from stomping
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities|Stomp", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShake> StompLandedCameraShake;
};
