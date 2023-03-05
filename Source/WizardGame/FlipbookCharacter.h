//
// Copyright (C) Taylor Beebe - All Rights Reserved Unauthorized copying of this repository,
// via any medium is strictly prohibited Proprietary and confidential 
// 
// Written by Taylor Beebe taylor.d.beebe@gmail.com, February 2023
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EightDirActorComponent.h"
#include "FlipbookCharacter.generated.h"

class UPaperFlipbookComponent;
class APlayerCameraManager;

UCLASS ()
class WIZARDGAME_API AFlipbookCharacter : public ACharacter
{
	GENERATED_BODY ()

	// Reference to the input context
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext *InputMappingContext;

	// Reference to the input action for looking
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction *LookInputAction;

	// Reference to the input action for moving
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction *MoveInputAction;

	// Reference to the input action for jumping
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction *JumpInputAction;

	// Reference to the input action for running
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction *RunInputAction;

	// Reference to the input action for aiming
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction *AimInputAction;

public:
	// Sets default values for this character's properties
	AFlipbookCharacter ();

protected:
	
	/**
		Run when the game starts or when spawned
	**/
	virtual void BeginPlay () override;

	/**
		Spawn actors for testing
	**/
	UFUNCTION (BlueprintCallable, Category = "Test Functions")
		void SpawnActors ();
	
	// Component which handles the functionality for updating the display and shadow flipbooks
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UEightDirActorComponent *EightDirActorComponent;

	// The speed at which the character moves when walking
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
		float FlipbookCharacterMaxWalkSpeed;

	// The speed at which the character moves when running
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
		float FlipbookCharacterMaxRunSpeed;

	// The actor class to spawn during the call to SpawnActors()
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "ActorSpawnTest")
		TSubclassOf<AActor> TestActorClass;

	// The flipbook visible to the camera
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbook", meta = (Tags = "DisplayFlipbook"))
		UPaperFlipbookComponent *DisplayFlipbook;

	// The flipbook invisible to the camera but which casts a shadow
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbook", meta = (Tags = "ShadowFlipbook"))
		UPaperFlipbookComponent *ShadowFlipbook;

public:
	/**
		Called every frame

		@param DeltaTime - The time since the last frame
	**/
	virtual void Tick (float DeltaTime) override;

	/**
		Initializes the player input component

		@param PlayerInputComponent - The player input component to initialize
	**/
	virtual void SetupPlayerInputComponent (class UInputComponent *PlayerInputComponent) override;

	/**
		Handles the movement input

		@param Value - The input value
	**/
	void Move (const FInputActionValue &Value);

	/**
	  Handles the release of the movement input
			
		@param Value - The input value
	**/
	void StopMoving (const FInputActionValue &Value);

	/**
		Handles the "look" input (ex. moving the right stick)
			
		@param Value - The input value
	**/
	void Look (const FInputActionValue &Value);

	/**
	  Handles the input which transitions the character from walking to running
			      
		@param Value - The input value
	**/
	void Run (const FInputActionValue &Value);

	/**
		Handles the release of input which transitions the character from walking to running

		@param Value: The input value
	**/
	void StopRunning (const FInputActionValue &Value);

	/**
		Handle the aim input (ex. pressing the left trigger)

		@param Value - The input value
	**/
	void Aim (const FInputActionValue &Value);

	/**
	  Handle the release of the aim input (ex. releasing the left trigger)
			
		@param Value - The input value
	**/
	void StopAiming (const FInputActionValue &Value);

private:
	
	// The character movement component
	UCharacterMovementComponent *CharacterMovementComponent;

	/**
		Gets the rotator of the vector drawn from the hit actor's location to this
		character's location. This should only be called if HitActorPtr is not null.

		@retval The override rotation to use for the character's display flipbook
	**/
	FRotator GetOverrideRotation ();
	
	// True if this character is aiming
	bool bIsAiming = false;

	// True if this character hit an actor with the "enemy" tag
	bool bDidHit = false;
	
	// The actor that this character hit while aiming
	TObjectPtr<AActor> HitActorPtr = nullptr;
	
	// The camera manager of the player
	APlayerCameraManager *LocalCameraManager;

};
