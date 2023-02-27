#pragma once

#include "CoreMinimal.h"
#include "EightDirFlipbookInterface.h"
#include "FloraInterface.h"
#include "EightDirActorComponent.h"
#include "Herb.generated.h"

class UCapsuleComponent;

UCLASS ()
class WIZARDGAME_API AHerb : public AActor, public IFloraInterface, public IEightDirFlipbookInterface
{
	GENERATED_BODY ()

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

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UPaperFlipbookComponent *DisplayFlipbook;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UPaperFlipbookComponent *ShadowFlipbook;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Root Component")
		UCapsuleComponent *CapsuleComponent;

public:
	AHerb ();

	virtual void Tick (float DeltaTime) override;

	virtual void BeginPlay () override;

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual bool RequiresWater ()  const override;

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual bool IsEdible ()  const override;

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual int GetNumDaysToGrow ()  const override;	

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual EFloraState GetFloraState ()  const override;

	UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
		virtual void SetFloraState (EFloraState NewState) override;

	//UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
	//	virtual UPaperFlipbook GetStateFlipbook (EFloraState State)  const override;

	//UFUNCTION (BlueprintCallable, Category = "Flora Interface Functions")
	//	virtual void SetStateFlipbook (EFloraState State, UPaperFlipbook Flipbook) override;

};
