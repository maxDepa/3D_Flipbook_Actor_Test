//
// Copyright (C) Taylor Beebe - All Rights Reserved Unauthorized copying of this repository,
// via any medium is strictly prohibited Proprietary and confidential 
// 
// Written by Taylor Beebe taylor.d.beebe@gmail.com, February 2023
//

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/StaticArray.h"
#include "EightDirActorComponent.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;
class USceneComponent;
class UPrimitiveComponent;
class APlayerCameraManager;
class ADirectionalLight;

////
// COMPASS ORIENTATION (normalized degrees)
// 
// NW        N       NE
//    -45    0    45
// W  -90         90  E
//    -135  180  135
// SW        S       SE
// 
// NOTE: Ensure any direction calculation for flipbooks is consistent
//		 with the above compass
////

#define NORTH 0.0f
#define NORTHEAST 45.0f
#define EAST 90.0f
#define SOUTHEAST 135.0f
#define SOUTH 180.0f
#define SOUTHWEST 225.0f
#define WEST 270.0f
#define NORTHWEST 315.0f

#define EIGHT_DIR_DIFF 22.5f
#define FOUR_DIR_DIFF 45.0f

// Flipbook direction enum
UENUM (BlueprintType)
enum class EEightDir : uint8 {
  North,
  Northeast,
  East,
  Southeast,
  South,
  Southwest,
  West,
  Northwest,
  EightDirMax
};

// Flipbook direction strings for debugging purposes
const FString DirectionStrings[] = {
  TEXT ("North"),
  TEXT ("Northeast"),
  TEXT ("East"),
  TEXT ("Southeast"),
  TEXT ("South"),
  TEXT ("Southwest"),
  TEXT ("West"),
  TEXT ("Northwest"),
  TEXT ("EightDirMax")
};

// Flipbook speed enum
UENUM (BlueprintType)
enum class EEightDirFlipbookSpeeds : uint8 {
  Stationary,
  Slow,
  Fast,
  SpeedMax
};

// Flipbook speed strings for debugging purposes
const FString FlipbookSpeedStrings[] = {
  TEXT ("Stationary"),
  TEXT ("Slow"),
  TEXT ("Fast"),
  TEXT ("EightDirFlipbookTypeMax")
};

// The following macros are used for the eight direction flipbook system
#define EIGHT_DIR_IS_NORTH(Yaw) (Yaw >= -22.5f && Yaw < 22.5f)
#define EIGHT_DIR_IS_NORTHEAST(Yaw) (Yaw >= 22.5f && Yaw < 67.5f)
#define EIGHT_DIR_IS_EAST(Yaw) (Yaw >= 67.5f && Yaw <= 112.5f)
#define EIGHT_DIR_IS_SOUTHEAST(Yaw) (Yaw > 112.5f && Yaw <= 157.5f)
#define EIGHT_DIR_IS_SOUTH(Yaw) (Yaw > 157.5f || Yaw <= -157.5f)
#define EIGHT_DIR_IS_SOUTHWEST(Yaw) (Yaw > -157.5f && Yaw <= -112.5f)
#define EIGHT_DIR_IS_WEST(Yaw) (Yaw > -112.5f && Yaw <= -67.5f)
#define EIGHT_DIR_IS_NORTHWEST(Yaw) (Yaw > -67.5f && Yaw <= -22.5f)

// The following macros are used for the four direction flipbook system
#define FOUR_DIR_IS_NORTH(Yaw) (Yaw >= -45.0f && Yaw < 45.0f)
#define FOUR_DIR_IS_EAST(Yaw) (Yaw >= 45.0f && Yaw <= 135.0f)
#define FOUR_DIR_IS_SOUTH(Yaw) (Yaw > 135.0 || Yaw <= -135.0)
#define FOUR_DIR_IS_WEST(Yaw) (Yaw > -135.0 && Yaw <= -45.0)

// Default flipbook path to use to ensure a NULL access does not occur
#define DEFAULT_FLIPBOOK_PATH TEXT ("/Game/PixelArt/Default/Flipbooks/DefaultFlipbook.DefaultFlipbook")

// Default flipbook speed
#define DEFAULT_FLIPBOOK_SLOW_SPEED 150.0f

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
class WIZARDGAME_API UEightDirActorComponent : public UActorComponent {
  GENERATED_BODY ()

public:
  // Sets default values for this component's properties
  UEightDirActorComponent ();

  /**
    Get the associated EEightDir enum value for the given Yaw value

    @param Yaw: The Yaw value to use to determine the direction
  **/
  UFUNCTION (BlueprintCallable, Category = "Eight Dir Actor Component Functions")
    EEightDir GetDirection (
    float Yaw
    );

  /**
    Update the display flipbook to be perpendicular to the vector from the camera to the owner of
    this component and the source flipbook of the display flipbook to be consistent with the direction
    the owner is facing (unless ComponentRotationOverride is specified). This function also updates
    the shadow flipbook to be perpendicular to the vector from the sun (or other directional light source)
    to the owner of this component. This function also updates the shadow flipbook to be consistent with
    the direction the owner is facing.

    To save resources, the flipbooks will not be updated if they are not being rendered unless ForceUpdate
    is true.

    @param ForceUpdate                - If true, the flipbook will always be updated
    @param SpeedOverride              - If specified, the flipbook speed will be set to this value
    @param ComponentRotationOverride  - If specified, this rotation will be used instead of basing
                                        the rotation on the owner's direction
  **/
  UFUNCTION (BlueprintCallable, Category = "Eight Dir Actor Component Functions")
    void UpdateDisplayAndShadowFlipbooks (
    bool ForceUpdate = false,
    float SpeedOverride = -1.0f,
    FRotator ComponentRotationOverride = FRotator::ZeroRotator
    );

  /**
    Update the display flipbook to be perpendicular to the vector from the camera to the owner of
    this component and the source flipbook of the display flipbook to be consistent with the direction
    the owner is facing (unless ComponentRotationOverride is specified).

    To save resources, this flipbook will not be updated if it is not being rendered unless ForceUpdate
    is true.

    @param ForceUpdate                - If true, the flipbook will always be updated
    @param SpeedOverride              - If specified, the flipbook speed will be set to this value
    @param ComponentRotationOverride  - If specified, this rotation will be used instead of basing
                                        the rotation on the owner's direction
  **/
  UFUNCTION (BlueprintCallable, Category = "Eight Dir Actor Component Functions")
    void UpdateDisplayFlipbook (
    bool ForceUpdate = false, 
    float SpeedOverride = -1.0f,
    FRotator ComponentRotationOverride = FRotator::ZeroRotator
    );

  /**
    Update the shadow flipbook to be perpendicular to the vector from the sun (or other
    directional light source) to the owner of this component. This function also updates the
    shadow flipbook to be consistent with the direction the owner is facing.

    To save resources, this flipbook will not be updated if it is not being rendered unless ForceUpdate
    is true.

    @param ForceUpdate                - If true, the flipbook will always be updated
    @param SpeedOverride              - If specified, the flipbook speed will be set to this value
    @param ComponentRotationOverride  - If specified, this rotation will be used instead of basing
                                        the rotation on the owner's direction
  **/
  UFUNCTION (BlueprintCallable, Category = "Eight Dir Actor Component Functions")
    void UpdateShadowFlipbook (
    bool ForceUpdate = false,
    float SpeedOverride = -1.0f
    );

  /**
    Setup all the defaults and globals for the attachment based on the input parameters

    @param FlipbookDirectory          - The directory from which to load the flipbooks
    @param HasStationaryFlipbooks     - True if the component has stationary animations
    @param HasSlowFlipbooks           - True if the component has slow animations
    @param HasFastFlipbooks           - True if the component has fast animations
    @param IsEightDir                 - True if the component has eight directions (otherwise four)
    @param IsTwoFlipbookRotation      - True if the component uses two flipbooks for rotation
    @param CastsShadow                - True if the component casts shadows
    @param SlowSpeed                  - The speed up to which the slow animations should be played
  **/
  UFUNCTION (BlueprintCallable, Category = "Eight Dir Actor Component Functions")
    void SetupAttachment (
    const FString &FlipbookDirectory,
    bool HasStationaryFlipbooks,
    bool HasSlowFlipbooks,
    bool HasFastFlipbooks,
    bool IsEightDir,
    bool IsTwoFlipbookRotation,
    bool CastsShadow = false,
    float SlowSpeed = -1.0f
    );

  // Array of the basic flipbooks indexable using the GET_FLIPBOOK_INDEX macro
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    TArray<UPaperFlipbook *> BasicFlipbooks;

protected:

  /**
    Run when the game starts or when spawned
  **/
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

  // True if the component has eight flipbooks for each basic flipbook speed
  bool bIsEightDirGlobal = false;

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

  /**
    Update the display and/or shadow flipbooks based on the input EEightDir values and speed

    @param DisplayFlipboookDirection  - The direction used to set the display flipbook
    @param ShadowFlipboookDirection   - The direction used to set the shadow flipbook
    @param Speed                      - The speed used to set the flipbooks
  **/
  void UpdateFlipbook (
    EEightDir DisplayFlipboookDirection,
    EEightDir ShadowFlipboookDirection,
    float Speed
  );

  /**
    Given a control rotation return the normalized relative rotation based on the owners rotation.
    If ComponentRotationOverride is specified, this rotation will be used instead of basing
    the rotation on the owner's direction.

    @param ControlRotation            - The control rotation to use
    @param ComponentRotationOverride  - If specified, this rotation will be used instead of basing
                                        the rotation on the owner's direction
  **/
  EEightDir GetFlipbookDirection (
    FRotator ControlRotation,
    FRotator ComponentRotationOverride = FRotator::ZeroRotator
  );

  /**
    Get the closest cardinal rotation based on the input Yaw

    @param Yaw                        - The Yaw to use
  **/
  float GetClosestRotation (float Yaw);

};
