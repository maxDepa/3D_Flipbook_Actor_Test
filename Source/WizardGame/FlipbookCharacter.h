// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext *InputMappingContext;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction *LookInputAction;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction *MoveInputAction;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction *JumpInputAction;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction *RunInputAction;

	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction *AimInputAction;

public:
	// Sets default values for this character's properties
	AFlipbookCharacter ();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay () override;

	UFUNCTION (BlueprintCallable, Category = "Test Functions")
		void SpawnActors ();
	
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UEightDirActorComponent *EightDirActorComponent;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
		float FlipbookCharacterMaxWalkSpeed;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
		float FlipbookCharacterMaxRunSpeed;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "ActorSpawnTest")
		TSubclassOf<AActor> TestActorClass;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
		UPaperFlipbookComponent *DisplayFlipbook;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
		UPaperFlipbookComponent *ShadowFlipbook;

public:
	virtual void Tick (float DeltaTime) override;

	virtual void SetupPlayerInputComponent (class UInputComponent *PlayerInputComponent) override;

	void Move (const FInputActionValue &Value);
	void StopMoving (const FInputActionValue &Value);
	void Look (const FInputActionValue &Value);
	void Run (const FInputActionValue &Value);
	void StopRunning (const FInputActionValue &Value);
	void Aim (const FInputActionValue &Value);
	void StopAiming (const FInputActionValue &Value);

private:
	UPROPERTY ()
		UCharacterMovementComponent *CharacterMovementComponent;

	bool IsAiming = false;

	APlayerCameraManager *LocalCameraManager;

};
