//
// Copyright (C) Taylor Beebe - All Rights Reserved Unauthorized copying of this repository,
// via any medium is strictly prohibited Proprietary and confidential 
// 
// Written by Taylor Beebe taylor.d.beebe@gmail.com, February 2023
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flora.generated.h"

class UPaperFlipbook;
class UEightDirActorComponent;
class UPaperFlipbookComponent;
class UCapsuleComponent;

// Possible states for some flora
UENUM (BlueprintType)
enum class EFloraState : uint8 {
	Destroyed,
	Seedling,
	Sapling,
	Young,
	Mature
};

// Strings for the flora states enum
const FString FloraStateStrings[] = {
	TEXT ("Destroyed"),
	TEXT ("Seedling"),
	TEXT ("Sapling"),
	TEXT ("Young"),
	TEXT ("Mature")
};

UCLASS()
class WIZARDGAME_API AFlora : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlora();

	/**
		Called every frame

		@param DeltaTime: The time since the last frame
	**/
	virtual void Tick (float DeltaTime) override;

	/**
		Run when the game starts or when spawned
	**/
	virtual void BeginPlay () override;

	/**
		Returns if the flora instance requires water

		@retval true - The flora instance requires water
		@retval false - The flora instance does not require water
	**/
	UFUNCTION (BlueprintCallable, Category = "Flora Functions")
		bool RequiresWater ();

	/**
		Returns if the flora instance can be eaten

		@retval true - The flora instance can be eaten
		@retval false - The flora instance cannot be eaten
	**/
	UFUNCTION (BlueprintCallable, Category = "Flora Functions")
		bool IsEdible ();

	/**
		Returns how many days the flora instance takes to grow from seedling to mature

		@retval int - The number of days to grow
	**/
	UFUNCTION (BlueprintCallable, Category = "Flora Functions")
		int GetNumDaysToGrow ();

	/**
		Get the current flora state enum value

		@retval EFloraState - The current flora state
	**/
	UFUNCTION (BlueprintCallable, Category = "Flora Functions")
		EFloraState GetFloraState ();

	/**
		Set the current flora state to the input NewState

		@param NewState - The new flora state
	**/
	UFUNCTION (BlueprintCallable, Category = "Flora Functions")
		void SetFloraState (EFloraState NewState);

	/**
		Increment the flora state enum
	**/
	UFUNCTION (BlueprintCallable, Category = "Flora Functions")
		void Grow ();

	/**
		Decrement the flora state enum
	**/
	UFUNCTION (BlueprintCallable, Category = "Flora Functions")
		void Shrink ();

	/**
		Destroy the flora state instance
	**/
	UFUNCTION (BlueprintCallable, Category = "Flora Functions")
		void Destroy ();

	//UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
	//	UPaperFlipbook GetStateFlipbook (EFloraState State);

	//UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
	//	void SetStateFlipbook (EFloraState State, UPaperFlipbook Flipbook);

protected:

	// The number of days it takes to grow from seedling to mature
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flora Interface Variables")
		int NumDaysToGrow;

	// The current state of the flora instance
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flora Interface Variables")
		EFloraState CurrentState;

	// If the flor instance requires water
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flora Interface Variables")
		bool bRequiresWater;

	// If the flora instance can be eaten
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flora Interface Variables")
		bool bIsEdible;

	// Component which handles the functionality for updating the display and shadow flipbooks
	// for the Seedling state
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UEightDirActorComponent *SeedlingActorComponent;

	// Component which handles the functionality for updating the display and shadow flipbooks
	// for the Sapling state
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UEightDirActorComponent *SaplingActorComponent;

	// Component which handles the functionality for updating the display and shadow flipbooks
	// for the Young state
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UEightDirActorComponent *YoungActorComponent;

	// Component which handles the functionality for updating the display and shadow flipbooks
	// for the Mature state
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UEightDirActorComponent *MatureActorComponent;

	// The display flipbook for the flora instance
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UPaperFlipbookComponent *DisplayFlipbook;

	// The shadow flipbook for the flora instance
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UPaperFlipbookComponent *ShadowFlipbook;

	// The root and capsule component for the flora instance
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Root Component")
		UCapsuleComponent *CapsuleComponent;

};
