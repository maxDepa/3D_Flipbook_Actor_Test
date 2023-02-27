// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FloraInterface.generated.h"

class UPaperFlipbook;

UENUM (BlueprintType)
enum class EFloraState : uint8 {
	Destroyed,
	Seedling,
	Sapling,
	Young,
	Mature
};

const FString FloraStateStrings[] = {
	TEXT ("Destroyed"),
	TEXT ("Seedling"),
	TEXT ("Sapling"),
	TEXT ("Young"),
	TEXT ("Mature")
};

UINTERFACE(MinimalAPI, NotBlueprintable)
class UFloraInterface : public UInterface
{
	GENERATED_BODY()
};

class WIZARDGAME_API IFloraInterface
{
	GENERATED_BODY()

public:
	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual bool RequiresWater () const = 0;
	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual bool IsEdible () const = 0;
	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual int GetNumDaysToGrow()  const = 0;
	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual EFloraState GetFloraState()  const = 0;
	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual void SetFloraState (EFloraState NewState) = 0;
	//UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
	//	virtual UPaperFlipbook GetStateFlipbook (EFloraState State)  const = 0;
	//UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
	//	virtual void SetStateFlipbook (EFloraState State, UPaperFlipbook Flipbook) = 0;
	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual void Grow ();

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual void Shrink ();

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual void Destroy ();
};
