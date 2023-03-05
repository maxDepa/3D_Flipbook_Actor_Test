//
// Copyright (C) Taylor Beebe - All Rights Reserved Unauthorized copying of this repository,
// via any medium is strictly prohibited Proprietary and confidential 
// 
// Written by Taylor Beebe taylor.d.beebe@gmail.com, February 2023
//

#include "EightDirActorComponent.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BillboardComponent.h"
#include "Engine/ObjectLibrary.h"

// UEightDirActorComponent constructor
UEightDirActorComponent::UEightDirActorComponent () {
  PrimaryComponentTick.bCanEverTick = false;

  BasicFlipbooks.Init (nullptr, static_cast<uint8>(EEightDir::EightDirMax) * static_cast<uint8>(EEightDirFlipbookSpeeds::SpeedMax));

  // Set all flipbooks to the default
  static ConstructorHelpers::FObjectFinder<UPaperFlipbook> DefaultFlipbook (DEFAULT_FLIPBOOK_PATH);
  if (DefaultFlipbook.Succeeded ()) {
    for (int i = 0; i < BasicFlipbooks.Num (); i++) {
      BasicFlipbooks[i] = DefaultFlipbook.Object;
    }
  } else {
    UE_LOG (LogTemp, Warning, TEXT ("DefaultFlipbook not found"));
  }
}

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
void UEightDirActorComponent::SetupAttachment (
  const FString &FlipbookDirectory,
  bool HasStationaryFlipbooks,
  bool HasSlowFlipbooks,
  bool HasFastFlipbooks,
  bool IsEightDir,
  bool IsTwoFlipbookRotation,
  bool CastsShadow,
  float SlowSpeed
) {

  bStationaryGlobal = HasStationaryFlipbooks;
  bSlowGlobal = HasSlowFlipbooks;
  bFastGlobal = HasFastFlipbooks;
  bIsEightDirGlobal = IsEightDir;
  bCastShadowGlobal = CastsShadow;
  SlowSpeedGlobal = SlowSpeed < 0.0f ? DEFAULT_FLIPBOOK_SLOW_SPEED : SlowSpeed;

  FString SearchTerm;
  int step = bIsEightDirGlobal ? 1 : 2;

  if (!IsTwoFlipbookRotation) {

    FString DirectoryWithNoTrailingSlash = FlipbookDirectory;
    UPaperFlipbook *Flipbook = nullptr;

    // Remove the trailing slash if it exists
    if (DirectoryWithNoTrailingSlash.EndsWith ("/")) {
      DirectoryWithNoTrailingSlash.RemoveAt (DirectoryWithNoTrailingSlash.Len () - 1);
    }

    // Load the slow flipbooks
    if (HasSlowFlipbooks) {
      for (int i = 0; i < static_cast<uint8>(EEightDir::EightDirMax); i += step) {
        SearchTerm = *FString::Printf (TEXT ("%s/slow_%s.slow_%s"), *DirectoryWithNoTrailingSlash, *DirectionStrings[i], *DirectionStrings[i]);
        Flipbook = Cast<UPaperFlipbook> (StaticLoadObject (UPaperFlipbook::StaticClass (), nullptr, *SearchTerm, nullptr, LOAD_None, nullptr));
        if (Flipbook) {
          BasicFlipbooks[GET_FLIPBOOK_INDEX (i, static_cast<uint8>(EEightDirFlipbookSpeeds::Slow))] = Flipbook;
        } else {
          UE_LOG (LogTemp, Warning, TEXT ("%s not found"), *SearchTerm);
        }
      }
    }

    // Load the fast flipbooks
    if (HasFastFlipbooks) {
      for (int i = 0; i < static_cast<uint8>(EEightDir::EightDirMax); i += step) {
        SearchTerm = *FString::Printf (TEXT ("%s/fast_%s.fast_%s"), *DirectoryWithNoTrailingSlash, *DirectionStrings[i], *DirectionStrings[i]);
        Flipbook = Cast<UPaperFlipbook> (StaticLoadObject (UPaperFlipbook::StaticClass (), nullptr, *SearchTerm, nullptr, LOAD_None, nullptr));
        if (Flipbook) {
          BasicFlipbooks[GET_FLIPBOOK_INDEX (i, static_cast<uint8>(EEightDirFlipbookSpeeds::Fast))] = Flipbook;
        } else {
          UE_LOG (LogTemp, Warning, TEXT ("%s not found"), *SearchTerm);
        }
      }
    }

    // Load the stationary flipbooks
    if (HasStationaryFlipbooks) {
      for (int i = 0; i < static_cast<uint8>(EEightDir::EightDirMax); i += step) {
        SearchTerm = *FString::Printf (TEXT ("%s/stationary_%s.stationary_%s"), *DirectoryWithNoTrailingSlash, *DirectionStrings[i], *DirectionStrings[i]);
        Flipbook = Cast<UPaperFlipbook> (StaticLoadObject (UPaperFlipbook::StaticClass (), nullptr, *SearchTerm, nullptr, LOAD_None, nullptr));
        if (Flipbook) {
          BasicFlipbooks[GET_FLIPBOOK_INDEX (i, static_cast<uint8>(EEightDirFlipbookSpeeds::Stationary))] = Flipbook;
        } else {
          UE_LOG (LogTemp, Warning, TEXT ("%s not found"), *SearchTerm);
        }
      }
    }
  } 
  // BEEBE TODO: Implement two flipbook rotation initialization logic for fast and slow speeds
  else {
    // Load the flipbooks for two flipbook rotation
    UObjectLibrary *FlipbookLibrary = UObjectLibrary::CreateLibrary (UPaperFlipbook::StaticClass (), false, GIsEditor);

    FlipbookLibrary->AddToRoot ();
    FlipbookLibrary->LoadAssetDataFromPath (FlipbookDirectory);
    FlipbookLibrary->LoadAssetsFromAssetData ();

    TArray<FAssetData> AssetDataList;
    FlipbookLibrary->GetAssetDataList (AssetDataList);

    // We expect there to be only two flipbooks in the input directory
    if (AssetDataList.Num () != 2) {
      UE_LOG (LogTemp, Warning, TEXT ("Directory %s can only have two flipbooks present"), *FlipbookDirectory);
      return;
    }

    // Load the flipbooks
    UPaperFlipbook *Flipbook1 = LoadObject<UPaperFlipbook> (nullptr, *AssetDataList[0].PackageName.ToString ());
    UPaperFlipbook *Flipbook2 = LoadObject<UPaperFlipbook> (nullptr, *AssetDataList[1].PackageName.ToString ());

    if (!Flipbook1) {
      UE_LOG (LogTemp, Warning, TEXT ("File %s is not valid!"), *AssetDataList[0].PackageName.ToString ());
    }

    if (!Flipbook2) {
      UE_LOG (LogTemp, Warning, TEXT ("File %s is not valid!"), *AssetDataList[1].PackageName.ToString ());
    }

    if (!(Flipbook1 && Flipbook2)) {
      return;
    }

    BasicFlipbooks[GET_FLIPBOOK_INDEX (EEightDir::North, EEightDirFlipbookSpeeds::Stationary)] = Flipbook1;
    BasicFlipbooks[GET_FLIPBOOK_INDEX (EEightDir::East, EEightDirFlipbookSpeeds::Stationary)] = bIsEightDirGlobal ? Flipbook1 : Flipbook2;
    BasicFlipbooks[GET_FLIPBOOK_INDEX (EEightDir::South, EEightDirFlipbookSpeeds::Stationary)] = Flipbook1;
    BasicFlipbooks[GET_FLIPBOOK_INDEX (EEightDir::West, EEightDirFlipbookSpeeds::Stationary)] = bIsEightDirGlobal ? Flipbook1 : Flipbook2;

    if (bIsEightDirGlobal) {
      BasicFlipbooks[GET_FLIPBOOK_INDEX (EEightDir::Northeast, EEightDirFlipbookSpeeds::Stationary)] = Flipbook2;
      BasicFlipbooks[GET_FLIPBOOK_INDEX (EEightDir::Southeast, EEightDirFlipbookSpeeds::Stationary)] = Flipbook2;
      BasicFlipbooks[GET_FLIPBOOK_INDEX (EEightDir::Southwest, EEightDirFlipbookSpeeds::Stationary)] = Flipbook2;
      BasicFlipbooks[GET_FLIPBOOK_INDEX (EEightDir::Northwest, EEightDirFlipbookSpeeds::Stationary)] = Flipbook2;
    }
  }
}

/**
  Update the display and/or shadow flipbooks based on the input EEightDir values and speed

  @param DisplayFlipboookDirection  - The direction used to set the display flipbook
  @param ShadowFlipboookDirection   - The direction used to set the shadow flipbook
  @param Speed                      - The speed used to set the flipbooks
**/
void UEightDirActorComponent::UpdateFlipbook (
  EEightDir DisplayFlipboookDirection,
  EEightDir ShadowFlipboookDirection,
  float Speed
) {

  UPaperFlipbook *FlipbookToUse[2] = {nullptr, nullptr};
  EEightDir Direction = DisplayFlipboookDirection;

  // If the display flipbook is set to EEightDir::EightDirMax, then we don't want to update the display flipbook
  // If the shadow flipbook is set to EEightDir::EightDirMax, then we don't want to update the shadow flipbook
  for (int i = (DisplayFlipboookDirection < EEightDir::EightDirMax ? 0 : 1);
       i < (bCastShadowGlobal && ShadowFlipboookDirection < EEightDir::EightDirMax ? 2 : 1);
       i++
       ) {

    // If we're on the shadow iteration, then we want to use the shadow flipbook direction
    if (i == SHADOW_ITERATION) {
      Direction = ShadowFlipboookDirection;
    }

    // If the speed is greater than the slow speed, then we want to use the fast flipbook
    if (Speed >= SlowSpeedGlobal + SLOW_SPEED_BUFFER && bFastGlobal) {
      FlipbookToUse[i] = BasicFlipbooks[GET_FLIPBOOK_INDEX (Direction, EEightDirFlipbookSpeeds::Fast)];

    // If the speed is greater than the minimum slow speed, then we want to use the slow flipbook
    } else if (Speed >= MIN_SLOW_SPEED && bSlowGlobal) {
      FlipbookToUse[i] = BasicFlipbooks[GET_FLIPBOOK_INDEX (Direction, EEightDirFlipbookSpeeds::Slow)];

    // Otherwise, we want to use the stationary flipbook
    } else {
      FlipbookToUse[i] = BasicFlipbooks[GET_FLIPBOOK_INDEX (Direction, EEightDirFlipbookSpeeds::Stationary)];
    }
  }

  // Update the display flipbook (if necessary)
  if (DisplayFlipbook && DisplayFlipboookDirection < EEightDir::EightDirMax) {
    DisplayFlipbook->SetFlipbook (FlipbookToUse[DISPLAY_ITERATION]);
  }

  // Update the shadow flipbook (if necessary)
  if (bCastShadowGlobal && ShadowFlipbook && ShadowFlipboookDirection < EEightDir::EightDirMax) {
    ShadowFlipbook->SetFlipbook (FlipbookToUse[SHADOW_ITERATION]);
  }
}

/**
  Run when the game starts or when spawned
**/
void UEightDirActorComponent::BeginPlay () {
  Super::BeginPlay ();

  // Populate the camera reference global
  CameraManagerGlobal = GetWorld ()->GetFirstPlayerController ()->PlayerCameraManager;
  if (!CameraManagerGlobal) {
    UE_LOG (LogTemp, Warning, TEXT ("Unable to initialize CameraManagerGlobal"));
  }

  // Populate the sun directional light global
  SunGlobal = Cast<ADirectionalLight> (
    UGameplayStatics::GetActorOfClass (GetWorld (), ADirectionalLight::StaticClass ())
    );
  if (!SunGlobal) {
    UE_LOG (LogTemp, Warning, TEXT ("Unable to initialize SunGlobal"))
  }

  AActor *Parent = GetOwner ();

  // Get the display and shadow flipbooks
  if (Parent) {
    TArray<UPaperFlipbookComponent *> Components;
    Parent->GetComponents<UPaperFlipbookComponent> (Components);

    for (UPaperFlipbookComponent *Component : Components) {
      if (Component->ComponentHasTag (TEXT ("DisplayFlipbook"))) {
        DisplayFlipbook = Component;
      } else if (Component->ComponentHasTag (TEXT ("ShadowFlipbook"))) {
        ShadowFlipbook = Component;
        ShadowFlipbook->CastShadow = true;
        ShadowFlipbook->bHiddenInGame = true;
        ShadowFlipbook->bCastHiddenShadow = true;
      }
    }
  }
  
  // Check that the display and shadow flipbooks are valid
  check (DisplayFlipbook);
  if (bCastShadowGlobal) {
    check (ShadowFlipbook);
  }

  // Get the root component
  RootComponentGlobal = Parent->GetRootComponent ();
  check (RootComponentGlobal);

  // Get the primitive cast of the root component
  if (RootComponentGlobal) {
    PrimitiveRootComponentGlobal = Cast<UPrimitiveComponent> (RootComponentGlobal);
    check (PrimitiveRootComponentGlobal);
  }
}

/**
  Get the associated EEightDir enum value for the given Yaw value

  @param Yaw: The Yaw value to use to determine the direction
**/
EEightDir UEightDirActorComponent::GetDirection (
  float Yaw
) {
  if (bIsEightDirGlobal) {
    if EIGHT_DIR_IS_NORTH (Yaw) {
      return EEightDir::North;
    } else if EIGHT_DIR_IS_NORTHEAST (Yaw) {
      return EEightDir::Northeast;
    } else if EIGHT_DIR_IS_EAST (Yaw) {
      return EEightDir::East;
    } else if EIGHT_DIR_IS_SOUTHEAST (Yaw) {
      return EEightDir::Southeast;
    } else if EIGHT_DIR_IS_SOUTH (Yaw) {
      return EEightDir::South;
    } else if EIGHT_DIR_IS_SOUTHWEST (Yaw) {
      return EEightDir::Southwest;
    } else if EIGHT_DIR_IS_WEST (Yaw) {
      return EEightDir::West;
    } else {
      return EEightDir::Northwest;
    }
  } else {
    if FOUR_DIR_IS_NORTH (Yaw) {
      return EEightDir::North;
    } else if FOUR_DIR_IS_EAST (Yaw) {
      return EEightDir::East;
    } else if FOUR_DIR_IS_SOUTH (Yaw) {
      return EEightDir::South;
    } else {
      return EEightDir::West;
    }
  }
}

/**
  Given a control rotation return the normalized relative rotation based on the owners rotation.
  If ComponentRotationOverride is specified, this rotation will be used instead of basing
  the rotation on the owner's direction.

  @param ControlRotation            - The control rotation to use
  @param ComponentRotationOverride  - If specified, this rotation will be used instead of basing
                                      the rotation on the owner's direction
**/
EEightDir UEightDirActorComponent::GetFlipbookDirection (
  FRotator ControlRotation,
  FRotator ComponentRotationOverride
) {
  return GetDirection (
    UKismetMathLibrary::NormalizedDeltaRotator (
    ComponentRotationOverride == FRotator::ZeroRotator ? RootComponentGlobal->GetComponentRotation () : ComponentRotationOverride,
    ControlRotation
  ).Yaw
  );
}

/**
  Get the closest cardinal rotation based on the input Yaw

  @param Yaw                        - The Yaw to use
**/
float UEightDirActorComponent::GetClosestRotation (float Yaw) {
  // Wrap the yaw to a value between 0 and 360
  Yaw = FMath::Fmod (Yaw, 360.0f);
  if (Yaw < 0.0f) {
    Yaw += 360.0f;
  }

  // Determine which direction the yaw is closest to
  if (bIsEightDirGlobal) {
    if (Yaw < NORTHEAST - EIGHT_DIR_DIFF) {
      return NORTH;
    } else if (Yaw < EAST - EIGHT_DIR_DIFF) {
      return NORTHEAST;
    } else if (Yaw < SOUTHEAST - EIGHT_DIR_DIFF) {
      return EAST;
    } else if (Yaw < SOUTH - EIGHT_DIR_DIFF) {
      return SOUTHEAST;
    } else if (Yaw < SOUTHWEST - EIGHT_DIR_DIFF) {
      return SOUTH;
    } else if (Yaw < WEST - EIGHT_DIR_DIFF) {
      return SOUTHWEST;
    } else if (Yaw < NORTHWEST - EIGHT_DIR_DIFF) {
      return WEST;
    } else {
      return NORTHWEST;
    }
  } else {
    // Return the closest of the four cardinal directions
    if (Yaw < EAST - FOUR_DIR_DIFF) {
      return NORTH;
    } else if (Yaw < SOUTH - FOUR_DIR_DIFF) {
      return EAST;
    } else if (Yaw < WEST - FOUR_DIR_DIFF) {
      return SOUTH;
    } else {
      return WEST;
    }
  }
}

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
void UEightDirActorComponent::UpdateDisplayFlipbook (
  bool ForceUpdate,
  float SpeedOverride,
  FRotator ComponentRotationOverride
) {

  // Only update the flipbook if it is being rendered
  if (DisplayFlipbook && (ForceUpdate || DisplayFlipbook->WasRecentlyRendered (0.1f))) {
    FRotator ControlRotation;
    EEightDir DisplayFlipbookDirection = EEightDir::North;
    FVector ControlVector;

    // Fetch the camera manager if the global is not populated
    if (!CameraManagerGlobal) {
      CameraManagerGlobal = GetWorld ()->GetFirstPlayerController ()->PlayerCameraManager;
    }

    if (CameraManagerGlobal) {

      // Determine the control rotation based on the vector from the camera to the owner
      ControlVector = GetOwner ()->GetActorLocation () - CameraManagerGlobal->GetCameraLocation ();
      ControlRotation = ControlVector.Rotation ();

      // Rotate the display flipbook
      DisplayFlipbook->SetWorldRotation (
        FRotator (0.0f, ControlRotation.Yaw + FLIPBOOK_ROTATIONAL_OFFSET, 0.0f)
      );

      float Speed = SpeedOverride;

      if (SpeedOverride < 0.0f && PrimitiveRootComponentGlobal) {
        Speed = PrimitiveRootComponentGlobal->GetPhysicsLinearVelocity ().Size ();
      }

      // Update the display flipbook
      UpdateFlipbook (
        GetFlipbookDirection (ControlRotation, ComponentRotationOverride),
        EEightDir::EightDirMax,
        Speed
      );
    }
  }
}

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
void UEightDirActorComponent::UpdateShadowFlipbook (
  bool ForceUpdate,
  float SpeedOverride
) {

  // Only update the flipbook if it is being rendered or the force update bool is set
  if (ShadowFlipbook && (ForceUpdate || ShadowFlipbook->WasRecentlyRendered (0.1f))) {
    FRotator ControlRotation;
    EEightDir ShadowFlipbookDirection = EEightDir::North;

    // Ensure shadow casting is enabled
    if (bCastShadowGlobal) {

      // Fetch the sun directional light if the global is not populated
      if (!SunGlobal) {
        SunGlobal = Cast<ADirectionalLight> (
          UGameplayStatics::GetActorOfClass (GetWorld (), ADirectionalLight::StaticClass ())
          );
      }

      if (SunGlobal) {

        // Rotate the shadow flipbook to be perpendicular to the sun directional light
        ControlRotation = SunGlobal->GetActorRotation ();
        ShadowFlipbook->SetWorldRotation (FRotator (0.0f, ControlRotation.Yaw + FLIPBOOK_ROTATIONAL_OFFSET, 0.0f));

        float Speed = SpeedOverride;

        if (SpeedOverride < 0.0f) {
          check (PrimitiveRootComponentGlobal);
          Speed = PrimitiveRootComponentGlobal->GetPhysicsLinearVelocity ().Size ();
        }

        // Update the shadow flipbook
        UpdateFlipbook (
          EEightDir::EightDirMax,
          GetFlipbookDirection (ControlRotation),
          Speed
        );
      }

      // Test rotating the sun
      //SunGlobal->SetActorRotation(FRotator(ControlRotation.Pitch, ControlRotation.Yaw + 1.0f, ControlRotation.Roll));
    }
  }
}

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
void UEightDirActorComponent::UpdateDisplayAndShadowFlipbooks (
  bool ForceUpdate,
  float SpeedOverride,
  FRotator ComponentRotationOverride
) {

  // Only update the flipbooks if they are being rendered or the force update bool is set
  if (DisplayFlipbook && (ForceUpdate || DisplayFlipbook->WasRecentlyRendered (0.1f))) {
    EEightDir ShadowFlipbookDirection = EEightDir::North;
    EEightDir DisplayFlipbookDirection = EEightDir::North;
    FRotator ControlRotation;
    FVector ControlVector;
    
    // Fetch the camera manager if the global is not populated
    if (!CameraManagerGlobal) {
      CameraManagerGlobal = GetWorld ()->GetFirstPlayerController ()->PlayerCameraManager;
    }

    if (CameraManagerGlobal) {
      
      // Determine the control rotation based on the vector from the camera to the owner
      ControlVector = GetOwner ()->GetActorLocation () - CameraManagerGlobal->GetCameraLocation ();
      ControlRotation = ControlVector.Rotation ();
      
      // Rotate the display flipbook
      DisplayFlipbook->SetWorldRotation (
        FRotator (0.0f, ControlRotation.Yaw + FLIPBOOK_ROTATIONAL_OFFSET, 0.0f)
      );

      // Determine the direction of the display flipbook
      DisplayFlipbookDirection = GetFlipbookDirection (
        ControlRotation, 
        ComponentRotationOverride
      );
    }
    
    if (ShadowFlipbook && bCastShadowGlobal) {

      // Fetch the sun directional light if the global is not populated
      if (!SunGlobal) {
        SunGlobal = Cast<ADirectionalLight> (
          UGameplayStatics::GetActorOfClass (GetWorld (), ADirectionalLight::StaticClass ())
          );
      }

      if (SunGlobal) {

        // Rotate the shadow flipbook to be perpendicular to the sun directional light
        ControlRotation = SunGlobal->GetActorRotation ();
        ShadowFlipbook->SetWorldRotation (
          FRotator (0.0f, ControlRotation.Yaw + FLIPBOOK_ROTATIONAL_OFFSET, 0.0f)
        );
        ShadowFlipbookDirection = GetFlipbookDirection (ControlRotation);
      }
    }

    float Speed = SpeedOverride;

    if (SpeedOverride < 0.0f) {
      check (PrimitiveRootComponentGlobal);
      Speed = PrimitiveRootComponentGlobal->GetPhysicsLinearVelocity ().Size ();
    }

    // Only update the flipbook(s) if the component has been rendered recently
    UpdateFlipbook (
      DisplayFlipbookDirection,
      ShadowFlipbookDirection,
      Speed
    );
  }
}
