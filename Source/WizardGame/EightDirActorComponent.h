// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EightDirFlipbookInterface.h"
#include "Containers/StaticArray.h"
#include "EightDirActorComponent.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;
class USceneComponent;
class UPrimitiveComponent;
class APlayerCameraManager;
class ADirectionalLight;

// Float representing a buffer for the slow speed to prevent jittering
#define SLOW_SPEED_BUFFER 10.0f

// Float representing the minimum speed for the slow animation
#define MIN_SLOW_SPEED 1.0f

// Float representing the loop iteration for updating the display flipbook
#define DISPLAY_ITERATION 0

// Float representing the loop iteration for updating the shadow flipbook
#define SHADOW_ITERATION 1

// Float representing a rotational offset required to ensure the flipbook is perpendicular to the camera
#define FLIPBOOK_ROTATIONAL_OFFSET 90.0f // BEEBE TODO: Change rotational zero point?

// Macro for getting the index of the desired flipbook based on the speed and direction
#define GET_FLIPBOOK_INDEX(Dir,Speed) \
    (((static_cast<uint8>(Dir) < static_cast<uint8>(EEightDir::EightDirMax)) && \
    (static_cast<uint8>(Speed) < static_cast<uint8>(EEightDirFlipbookSpeeds::SpeedMax))) ? \
    ((static_cast<uint8>(Speed) * static_cast<uint8>(EEightDir::EightDirMax)) + static_cast<uint8>(Dir)) : \
    (0))

UCLASS (ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WIZARDGAME_API UEightDirActorComponent : public UActorComponent, public IEightDirFlipbookInterface {
  GENERATED_BODY ()

public:
  // Sets default values for this component's properties
  UEightDirActorComponent ();

  UFUNCTION (BlueprintCallable, Category = "Functions")
    void UpdateDisplayAndShadowFlipbooks (bool ForceUpdate = false, float SpeedOverride = -1.0f);

  UFUNCTION (BlueprintCallable, Category = "Functions")
    void UpdateDisplayFlipbook (bool ForceUpdate = false, float SpeedOverride = -1.0f);

  UFUNCTION (BlueprintCallable, Category = "Functions")
    void UpdateShadowFlipbook (bool ForceUpdate = false, float SpeedOverride = -1.0f);

  UFUNCTION (BlueprintCallable, Category = "Functions")
    void LoadFlipbooksFromDirectory (const FString &Directory, bool stationary, bool slow, bool fast, bool TwoFlipbookRotation = false);

  // Indirection for the USceneCompoenent SetupAttachment function
  UFUNCTION (BlueprintCallable, Category = "Functions")
    void SetupAttachment (
    USceneComponent *ActorRootComponent,
    UPaperFlipbookComponent *DisplayFlipbookComponent,
    UPaperFlipbookComponent *ShadowFlipbookComponent,
    bool CastShadow = false,
    float SlowSpeed = -1.0f
    );

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    TArray<UPaperFlipbook *> BasicFlipbooks;

protected:
  virtual void BeginPlay () override;

private:

  // True if the component has stationary animations
  bool bStationaryGlobal = false;

  // True if the component has slow animations
  bool bSlowGlobal = false;

  // True if the component has fast animations
  bool bFastGlobal = false;

  // True if the component casts shadows
  bool bCastShadowGlobal = false;

  // Float representing the speed separating slow and fast animations
  float SlowSpeedGlobal = DEFAULT_FLIPBOOK_SLOW_SPEED;

  // Reference to the root component of the attached actor
  TObjectPtr<USceneComponent> RootComponentGlobal;

  // Reference to a primitive cast of the root component of the attached actor
  TObjectPtr <UPrimitiveComponent> PrimitiveRootComponentGlobal;

  // Reference to the player camera manager
  TObjectPtr <APlayerCameraManager> CameraManagerGlobal;

  // Reference to the Sun directional light
  TObjectPtr <ADirectionalLight> SunGlobal;

  // Reference to the display flipbook component
  TObjectPtr <UPaperFlipbookComponent> DisplayFlipbook;

  // Reference to the shadow flipbook component
  TObjectPtr <UPaperFlipbookComponent> ShadowFlipbook;

  void UpdateFlipbook (
    EEightDir DisplayFlipboookDirection,
    EEightDir ShadowFlipboookDirection,
    float Speed
  );

  EEightDir GetFlipbookDirection (FRotator ControlRotation, FRotator ComponentRotation);

};
