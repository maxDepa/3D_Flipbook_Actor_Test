// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flora.generated.h"

class UPaperFlipbook;
class UEightDirActorComponent;
class UPaperFlipbookComponent;
class UCapsuleComponent;

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

UCLASS()
class WIZARDGAME_API AFlora : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlora();

	virtual void Tick (float DeltaTime) override;

	virtual void BeginPlay () override;

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		bool RequiresWater ();

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		bool IsEdible ();

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		int GetNumDaysToGrow ();

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		EFloraState GetFloraState ();

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		void SetFloraState (EFloraState NewState);

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		void Grow ();

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		void Shrink ();

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		void Destroy ();

	//UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
	//	UPaperFlipbook GetStateFlipbook (EFloraState State);

	//UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
	//	void SetStateFlipbook (EFloraState State, UPaperFlipbook Flipbook);

protected:

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flora Interface Variables")
		int NumDaysToGrow;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flora Interface Variables")
		EFloraState CurrentState;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flora Interface Variables")
		bool bRequiresWater;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flora Interface Variables")
		bool bIsEdible;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UEightDirActorComponent *SeedlingActorComponent;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UEightDirActorComponent *SaplingActorComponent;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UEightDirActorComponent *YoungActorComponent;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UEightDirActorComponent *MatureActorComponent;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks", meta = (Tags = "DisplayFlipbook"))
		UPaperFlipbookComponent *DisplayFlipbook;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks", meta = (Tags = "ShadowFlipbook"))
		UPaperFlipbookComponent *ShadowFlipbook;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Root Component")
		UCapsuleComponent *CapsuleComponent;

};
