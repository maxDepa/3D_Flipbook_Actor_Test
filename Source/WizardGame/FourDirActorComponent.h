// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FourDirFlipbookInterface.h"
#include "FourDirActorComponent.generated.h"

class UPaperFlipbookComponent;
class UPaperFlipbook;

UCLASS (ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WIZARDGAME_API UFourDirActorComponent : public UActorComponent, public IFourDirFlipbookInterface {
  GENERATED_BODY ()

public:
  UFourDirActorComponent ();

  UFUNCTION (BlueprintCallable, Category = "Functions")
    void UpdateFlipbook (UPaperFlipbookComponent *FlipbookToUpdate, EFourDir Direction, float SlowSpeed = 0, float Speed = 0);

  UFUNCTION (BlueprintCallable, Category = "Functions")
    void LoadFlipbooksFromDirectory (const FString &Directory, bool stationary, bool slow, bool fast, bool TwoFlipbookRotation = false);

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *StationaryNorthFlipbook;

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *StationaryEastFlipbook;

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *StationarySouthFlipbook;

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *StationaryWestFlipbook;

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *SlowNorthFlipbook;

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *SlowEastFlipbook;

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *SlowSouthFlipbook;

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *SlowWestFlipbook;

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *FastNorthFlipbook;

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *FastEastFlipbook;

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *FastSouthFlipbook;

  UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UPaperFlipbook *FastWestFlipbook;

private:
  bool bStationaryGlobal = false;
  bool bSlowGlobal = false;
  bool bFastGlobal = false;

};
