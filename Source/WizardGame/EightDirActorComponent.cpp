// Fill out your copyright notice in the Description page of Project Settings.


#include "EightDirActorComponent.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BillboardComponent.h"
#include "Engine/ObjectLibrary.h"

// Sets default values for this component's properties
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

    if (DirectoryWithNoTrailingSlash.EndsWith ("/")) {
      DirectoryWithNoTrailingSlash.RemoveAt (DirectoryWithNoTrailingSlash.Len () - 1);
    }

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
  } else {

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

void UEightDirActorComponent::UpdateFlipbook (
  EEightDir DisplayFlipboookDirection,
  EEightDir ShadowFlipboookDirection,
  float Speed
) {
  UPaperFlipbook *FlipbookToUse[2] = {nullptr, nullptr};
  EEightDir Direction = DisplayFlipboookDirection;

  for (int i = (DisplayFlipboookDirection < EEightDir::EightDirMax ? 0 : 1);
       i < (bCastShadowGlobal && ShadowFlipboookDirection < EEightDir::EightDirMax ? 2 : 1);
       i++
       ) {

    if (i == SHADOW_ITERATION) {
      Direction = ShadowFlipboookDirection;
    }

    if (Speed >= SlowSpeedGlobal + SLOW_SPEED_BUFFER && bFastGlobal) {
      FlipbookToUse[i] = BasicFlipbooks[GET_FLIPBOOK_INDEX (Direction, EEightDirFlipbookSpeeds::Fast)];
    } else if (Speed >= MIN_SLOW_SPEED && bSlowGlobal) {
      FlipbookToUse[i] = BasicFlipbooks[GET_FLIPBOOK_INDEX (Direction, EEightDirFlipbookSpeeds::Slow)];
    } else {
      FlipbookToUse[i] = BasicFlipbooks[GET_FLIPBOOK_INDEX (Direction, EEightDirFlipbookSpeeds::Stationary)];
    }
  }

  if (DisplayFlipbook && DisplayFlipboookDirection < EEightDir::EightDirMax) {
    DisplayFlipbook->SetFlipbook (FlipbookToUse[DISPLAY_ITERATION]);
  }

  if (bCastShadowGlobal && ShadowFlipbook && ShadowFlipboookDirection < EEightDir::EightDirMax) {
    ShadowFlipbook->SetFlipbook (FlipbookToUse[SHADOW_ITERATION]);
  }
}

void UEightDirActorComponent::BeginPlay () {
  Super::BeginPlay ();

  // Populate the camera reference global
  CameraManagerGlobal = GetWorld ()->GetFirstPlayerController ()->PlayerCameraManager;
  if (!CameraManagerGlobal) {
    UE_LOG (LogTemp, Warning, TEXT ("Unable to initialize CameraManagerGlobal"));
  }

  // Populate the "sun" directional light global
  SunGlobal = Cast<ADirectionalLight> (
    UGameplayStatics::GetActorOfClass (GetWorld (), ADirectionalLight::StaticClass ())
    );
  if (!SunGlobal) {
    UE_LOG (LogTemp, Warning, TEXT ("Unable to initialize SunGlobal"))
  }

  AActor *Parent = GetOwner ();

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

  check (DisplayFlipbook);
  if (bCastShadowGlobal) {
    check (ShadowFlipbook);
  }

  RootComponentGlobal = Parent->GetRootComponent ();
  check (RootComponentGlobal);

  if (RootComponentGlobal) {
    PrimitiveRootComponentGlobal = Cast<UPrimitiveComponent> (RootComponentGlobal);
    check (PrimitiveRootComponentGlobal);
  }
}

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

EEightDir UEightDirActorComponent::GetFlipbookDirection (
  FRotator ControlRotation
) {
  return GetDirection (
    UKismetMathLibrary::NormalizedDeltaRotator (RootComponentGlobal->GetComponentRotation (), ControlRotation).Yaw
  );
}

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

void UEightDirActorComponent::UpdateDisplayFlipbook (
  bool ForceUpdate,
  float SpeedOverride
) {
  if (DisplayFlipbook && (ForceUpdate || DisplayFlipbook->WasRecentlyRendered (0.1f))) {
    FRotator ControlRotation;
    EEightDir DisplayFlipbookDirection = EEightDir::North;
    FVector ControlVector;

    if (!CameraManagerGlobal) {
      CameraManagerGlobal = GetWorld ()->GetFirstPlayerController ()->PlayerCameraManager;
    }

    // BEEBE TODO: Remove?
    if (CameraManagerGlobal) {

      ControlVector = GetOwner ()->GetActorLocation () - CameraManagerGlobal->GetCameraLocation ();
      ControlRotation = ControlVector.Rotation ();

      // Rotate the display flipbook
      DisplayFlipbook->SetWorldRotation (
        FRotator (0.0f, ControlRotation.Yaw + FLIPBOOK_ROTATIONAL_OFFSET, 0.0f)
      );

      float Speed = SpeedOverride;

      if (SpeedOverride < 0.0f) {
        check (PrimitiveRootComponentGlobal);
        Speed = PrimitiveRootComponentGlobal->GetPhysicsLinearVelocity ().Size ();
      }

      UpdateFlipbook (
        GetFlipbookDirection (ControlRotation),
        EEightDir::EightDirMax,
        Speed
      );

    }
  }
}

void UEightDirActorComponent::UpdateShadowFlipbook (
  bool ForceUpdate,
  float SpeedOverride
) {
  if (ShadowFlipbook && (ForceUpdate || ShadowFlipbook->WasRecentlyRendered (0.1f))) {
    FRotator ControlRotation;
    EEightDir ShadowFlipbookDirection = EEightDir::North;

    if (bCastShadowGlobal) {

      // BEEBE TODO: Remove?
      if (!SunGlobal) {
        SunGlobal = Cast<ADirectionalLight> (
          UGameplayStatics::GetActorOfClass (GetWorld (), ADirectionalLight::StaticClass ())
          );
      }

      if (SunGlobal) {
        // Rotate the shadow flipbook
        ControlRotation = SunGlobal->GetActorRotation ();
        ShadowFlipbook->SetWorldRotation (FRotator (0.0f, ControlRotation.Yaw + FLIPBOOK_ROTATIONAL_OFFSET, 0.0f));

        float Speed = SpeedOverride;

        if (SpeedOverride < 0.0f) {
          check (PrimitiveRootComponentGlobal);
          Speed = PrimitiveRootComponentGlobal->GetPhysicsLinearVelocity ().Size ();
        }

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

void UEightDirActorComponent::UpdateDisplayAndShadowFlipbooks (
  bool ForceUpdate,
  float SpeedOverride
) {
  if (DisplayFlipbook && (ForceUpdate || DisplayFlipbook->WasRecentlyRendered (0.1f))) {
    EEightDir ShadowFlipbookDirection = EEightDir::North;
    EEightDir DisplayFlipbookDirection = EEightDir::North;
    FRotator ControlRotation;
    FVector ControlVector;

    // BEEBE TODO: Remove?
    if (!CameraManagerGlobal) {
      CameraManagerGlobal = GetWorld ()->GetFirstPlayerController ()->PlayerCameraManager;
    }

    if (CameraManagerGlobal) {
      
      ControlVector = GetOwner ()->GetActorLocation () - CameraManagerGlobal->GetCameraLocation ();
      ControlRotation = ControlVector.Rotation ();
      
      // Rotate the display flipbook
      DisplayFlipbook->SetWorldRotation (
        FRotator (0.0f, ControlRotation.Yaw + FLIPBOOK_ROTATIONAL_OFFSET, 0.0f)
      );
      DisplayFlipbookDirection = GetFlipbookDirection (ControlRotation);
    }

    if (ShadowFlipbook && bCastShadowGlobal) {
      // BEEBE TODO: Remove?
      if (!SunGlobal) {
        SunGlobal = Cast<ADirectionalLight> (
          UGameplayStatics::GetActorOfClass (GetWorld (), ADirectionalLight::StaticClass ())
          );
      }

      if (SunGlobal) {
        // Rotate the shadow flipbook
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
