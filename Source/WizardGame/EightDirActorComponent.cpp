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

  // Set all flipbooks to the default
  static ConstructorHelpers::FObjectFinder<UPaperFlipbook> DefaultFlipbook (DEFAULT_FLIPBOOK_PATH);
  if (DefaultFlipbook.Succeeded ()) {
    StationaryNorthFlipbook = DefaultFlipbook.Object;
    StationaryNortheastFlipbook = DefaultFlipbook.Object;
    StationaryEastFlipbook = DefaultFlipbook.Object;
    StationarySoutheastFlipbook = DefaultFlipbook.Object;
    StationarySouthFlipbook = DefaultFlipbook.Object;
    StationarySouthwestFlipbook = DefaultFlipbook.Object;
    StationaryWestFlipbook = DefaultFlipbook.Object;
    StationaryNorthwestFlipbook = DefaultFlipbook.Object;
    SlowNorthFlipbook = DefaultFlipbook.Object;
    SlowNortheastFlipbook = DefaultFlipbook.Object;
    SlowEastFlipbook = DefaultFlipbook.Object;
    SlowSoutheastFlipbook = DefaultFlipbook.Object;
    SlowSouthFlipbook = DefaultFlipbook.Object;
    SlowSouthwestFlipbook = DefaultFlipbook.Object;
    SlowWestFlipbook = DefaultFlipbook.Object;
    SlowNorthwestFlipbook = DefaultFlipbook.Object;
    FastNorthFlipbook = DefaultFlipbook.Object;
    FastNortheastFlipbook = DefaultFlipbook.Object;
    FastEastFlipbook = DefaultFlipbook.Object;
    FastSoutheastFlipbook = DefaultFlipbook.Object;
    FastSouthFlipbook = DefaultFlipbook.Object;
    FastSouthwestFlipbook = DefaultFlipbook.Object;
    FastWestFlipbook = DefaultFlipbook.Object;
    FastNorthwestFlipbook = DefaultFlipbook.Object;
  } else {
    UE_LOG (LogTemp, Warning, TEXT ("DefaultFlipbook not found"));
  }
}

void UEightDirActorComponent::LoadFlipbooksFromDirectory (const FString &Directory, bool Stationary, bool Slow, bool Fast, bool TwoFlipbookRotation) {

  bStationaryGlobal = Stationary;
  bSlowGlobal = Slow;
  bFastGlobal = Fast;

  if (!TwoFlipbookRotation) {
    if (Slow) {
      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> NorthAsset (*FString::Printf (TEXT ("%s/slow_north.slow_north"), *Directory));
      if (NorthAsset.Succeeded ()) {
        SlowNorthFlipbook = NorthAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("NorthAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> NorthEastAsset (*FString::Printf (TEXT ("%s/slow_northeast.slow_northeast"), *Directory));
      if (NorthEastAsset.Succeeded ()) {
        SlowNortheastFlipbook = NorthEastAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("NorthEastAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> NorthWestAsset (*FString::Printf (TEXT ("%s/slow_northwest.slow_northwest"), *Directory));
      if (NorthWestAsset.Succeeded ()) {
        SlowNorthwestFlipbook = NorthWestAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("NorthWestAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> WestAsset (*FString::Printf (TEXT ("%s/slow_west.slow_west"), *Directory));
      if (WestAsset.Succeeded ()) {
        SlowWestFlipbook = WestAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("WestAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> EastAsset (*FString::Printf (TEXT ("%s/slow_east.slow_east"), *Directory));
      if (EastAsset.Succeeded ()) {
        SlowEastFlipbook = EastAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("EastAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> SouthAsset (*FString::Printf (TEXT ("%s/slow_south.slow_south"), *Directory));
      if (SouthAsset.Succeeded ()) {
        SlowSouthFlipbook = SouthAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("SouthAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> SouthEastAsset (*FString::Printf (TEXT ("%s/slow_southeast.slow_southeast"), *Directory));
      if (SouthEastAsset.Succeeded ()) {
        SlowSoutheastFlipbook = SouthEastAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("SouthEastAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> SouthWestAsset (*FString::Printf (TEXT ("%s/slow_southwest.slow_southwest"), *Directory));
      if (SouthWestAsset.Succeeded ()) {
        SlowSouthwestFlipbook = SouthWestAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("SouthWestAsset not found"))
      }
    }

    if (Fast) {
      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FastNorthAsset (*FString::Printf (TEXT ("%s/fast_north.fast_north"), *Directory));
      if (FastNorthAsset.Succeeded ()) {
        FastNorthFlipbook = FastNorthAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("FastNorthAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FastNorthEastAsset (*FString::Printf (TEXT ("%s/fast_northeast.fast_northeast"), *Directory));
      if (FastNorthEastAsset.Succeeded ()) {
        FastNortheastFlipbook = FastNorthEastAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("FastNorthEastAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FastNorthWestAsset (*FString::Printf (TEXT ("%s/fast_northwest.fast_northwest"), *Directory));
      if (FastNorthWestAsset.Succeeded ()) {
        FastNorthwestFlipbook = FastNorthWestAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("FastNorthWestAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FastWestAsset (*FString::Printf (TEXT ("%s/fast_west.fast_west"), *Directory));
      if (FastWestAsset.Succeeded ()) {
        FastWestFlipbook = FastWestAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("FastWestAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FastEastAsset (*FString::Printf (TEXT ("%s/fast_east.fast_east"), *Directory));
      if (FastEastAsset.Succeeded ()) {
        FastEastFlipbook = FastEastAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("FastEastAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FastSouthAsset (*FString::Printf (TEXT ("%s/fast_south.fast_south"), *Directory));
      if (FastSouthAsset.Succeeded ()) {
        FastSouthFlipbook = FastSouthAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("FastSouthAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FastSouthEastAsset (*FString::Printf (TEXT ("%s/fast_southeast.fast_southeast"), *Directory));
      if (FastSouthEastAsset.Succeeded ()) {
        FastSoutheastFlipbook = FastSouthEastAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("FastSouthEastAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FastSouthWestAsset (*FString::Printf (TEXT ("%s/fast_southwest.fast_southwest"), *Directory));
      if (FastSouthWestAsset.Succeeded ()) {
        FastSouthwestFlipbook = FastSouthWestAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("FastSouthWestAsset not found"));
      }
    }

    if (Stationary) {
      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> StationaryNorthAsset (*FString::Printf (TEXT ("%s/stationary_north.stationary_north"), *Directory));
      if (StationaryNorthAsset.Succeeded ()) {
        StationaryNorthFlipbook = StationaryNorthAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("StationaryNorthAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> StationaryNorthEastAsset (*FString::Printf (TEXT ("%s/stationary_northeast.stationary_northeast"), *Directory));
      if (StationaryNorthEastAsset.Succeeded ()) {
        StationaryNortheastFlipbook = StationaryNorthEastAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("StationaryNorthEastAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> StationaryNorthWestAsset (*FString::Printf (TEXT ("%s/stationary_northwest.stationary_northwest"), *Directory));
      if (StationaryNorthWestAsset.Succeeded ()) {
        StationaryNorthwestFlipbook = StationaryNorthWestAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("StationaryNorthWestAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> StationaryWestAsset (*FString::Printf (TEXT ("%s/stationary_west.stationary_west"), *Directory));
      if (StationaryWestAsset.Succeeded ()) {
        StationaryWestFlipbook = StationaryWestAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("StationaryWestAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> StationaryEastAsset (*FString::Printf (TEXT ("%s/stationary_east.stationary_east"), *Directory));
      if (StationaryEastAsset.Succeeded ()) {
        StationaryEastFlipbook = StationaryEastAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("StationaryEastAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> StationarySouthAsset (*FString::Printf (TEXT ("%s/stationary_south.stationary_south"), *Directory));
      if (StationarySouthAsset.Succeeded ()) {
        StationarySouthFlipbook = StationarySouthAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("StationarySouthAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> StationarySouthEastAsset (*FString::Printf (TEXT ("%s/stationary_southeast.stationary_southeast"), *Directory));
      if (StationarySouthEastAsset.Succeeded ()) {
        StationarySoutheastFlipbook = StationarySouthEastAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("StationarySouthEastAsset not found"));
      }

      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> StationarySouthWestAsset (*FString::Printf (TEXT ("%s/stationary_southwest.stationary_southwest"), *Directory));
      if (StationarySouthWestAsset.Succeeded ()) {
        StationarySouthwestFlipbook = StationarySouthWestAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("StationarySouthWestAsset not found"));
      }
    }
  } else {

    UObjectLibrary *FlipbookLibrary = UObjectLibrary::CreateLibrary (UPaperFlipbook::StaticClass (), false, GIsEditor);

    FlipbookLibrary->AddToRoot ();
    FlipbookLibrary->LoadAssetDataFromPath (Directory);
    FlipbookLibrary->LoadAssetsFromAssetData ();

    TArray<FAssetData> AssetDataList;
    FlipbookLibrary->GetAssetDataList (AssetDataList);

    // We expect there to be only two flipbooks in the input directory
    if (AssetDataList.Num () != 2) {
      UE_LOG (LogTemp, Warning, TEXT ("Directory %s can only have two flipbooks present"), *Directory);
      return;
    }

    UPaperFlipbook *Flipbook1 = LoadObject<UPaperFlipbook> (nullptr, *AssetDataList[0].PackageName.ToString ());
    UPaperFlipbook *Flipbook2 = LoadObject<UPaperFlipbook> (nullptr, *AssetDataList[1].PackageName.ToString ());

    if (Flipbook1) {
      StationaryNorthFlipbook = Flipbook1;
      StationaryEastFlipbook = Flipbook1;
      StationarySouthFlipbook = Flipbook1;
      StationaryWestFlipbook = Flipbook1;
    } else {
      UE_LOG (LogTemp, Warning, TEXT ("File %s is not valid!"), *AssetDataList[0].PackageName.ToString ());
    }

    if (Flipbook2) {
      StationaryNortheastFlipbook = Flipbook2;
      StationarySoutheastFlipbook = Flipbook2;
      StationarySouthwestFlipbook = Flipbook2;
      StationaryNorthwestFlipbook = Flipbook2;
    } else {
      UE_LOG (LogTemp, Warning, TEXT ("File %s is not valid!"), *AssetDataList[1].PackageName.ToString ());
    }
  }
}

void UEightDirActorComponent::SetupAttachment (
  USceneComponent *ActorRootComponent,
  UPaperFlipbookComponent *DisplayFlipbookComponent,
  UPaperFlipbookComponent *ShadowFlipbookComponent,
  bool CastShadow,
  float SlowSpeed
) {
  DisplayFlipbook = DisplayFlipbookComponent;

  if (CastShadow && ShadowFlipbookComponent) {
    ShadowFlipbook = ShadowFlipbookComponent;
    ShadowFlipbook->CastShadow = true;
    ShadowFlipbook->bHiddenInGame = true;
    ShadowFlipbook->bCastHiddenShadow = true;
  }
  bCastShadowGlobal = CastShadow;
  RootComponentGlobal = ActorRootComponent;
  SlowSpeedGlobal = SlowSpeed < 0.0f ? DEFAULT_FLIPBOOK_SLOW_SPEED : SlowSpeed;
  PrimitiveRootComponentGlobal = Cast<UPrimitiveComponent> (ActorRootComponent);
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
      switch (Direction) {
        case EEightDir::North:
          FlipbookToUse[i] = FastNorthFlipbook;
          break;
        case EEightDir::Northeast:
          FlipbookToUse[i] = FastNortheastFlipbook;
          break;
        case EEightDir::East:
          FlipbookToUse[i] = FastEastFlipbook;
          break;
        case EEightDir::Southeast:
          FlipbookToUse[i] = FastSoutheastFlipbook;
          break;
        case EEightDir::South:
          FlipbookToUse[i] = FastSouthFlipbook;
          break;
        case EEightDir::Southwest:
          FlipbookToUse[i] = FastSouthwestFlipbook;
          break;
        case EEightDir::West:
          FlipbookToUse[i] = FastWestFlipbook;
          break;
        case EEightDir::Northwest:
          FlipbookToUse[i] = FastNorthwestFlipbook;
          break;
        default:
          UE_LOG (LogTemp, Warning, TEXT ("Invalid direction!"));
          FlipbookToUse[i] = FastNorthFlipbook;
          break;
      }
    } else if (Speed >= MIN_SLOW_SPEED && bSlowGlobal) {
      switch (Direction) {
        case EEightDir::North:
          FlipbookToUse[i] = SlowNorthFlipbook;
          break;
        case EEightDir::Northeast:
          FlipbookToUse[i] = SlowNortheastFlipbook;
          break;
        case EEightDir::East:
          FlipbookToUse[i] = SlowEastFlipbook;
          break;
        case EEightDir::Southeast:
          FlipbookToUse[i] = SlowSoutheastFlipbook;
          break;
        case EEightDir::South:
          FlipbookToUse[i] = SlowSouthFlipbook;
          break;
        case EEightDir::Southwest:
          FlipbookToUse[i] = SlowSouthwestFlipbook;
          break;
        case EEightDir::West:
          FlipbookToUse[i] = SlowWestFlipbook;
          break;
        case EEightDir::Northwest:
          FlipbookToUse[i] = SlowNorthwestFlipbook;
          break;
        default:
          UE_LOG (LogTemp, Warning, TEXT ("Invalid direction!"));
          FlipbookToUse[i] = SlowNorthFlipbook;
          break;
      }
    } else {
      switch (Direction) {
        case EEightDir::North:
          FlipbookToUse[i] = StationaryNorthFlipbook;
          break;
        case EEightDir::Northeast:
          FlipbookToUse[i] = StationaryNortheastFlipbook;
          break;
        case EEightDir::East:
          FlipbookToUse[i] = StationaryEastFlipbook;
          break;
        case EEightDir::Southeast:
          FlipbookToUse[i] = StationarySoutheastFlipbook;
          break;
        case EEightDir::South:
          FlipbookToUse[i] = StationarySouthFlipbook;
          break;
        case EEightDir::Southwest:
          FlipbookToUse[i] = StationarySouthwestFlipbook;
          break;
        case EEightDir::West:
          FlipbookToUse[i] = StationaryWestFlipbook;
          break;
        case EEightDir::Northwest:
          FlipbookToUse[i] = StationaryNorthwestFlipbook;
          break;
        default:
          UE_LOG (LogTemp, Warning, TEXT ("Invalid direction!"));
          FlipbookToUse[i] = StationaryNorthFlipbook;
          break;
      }
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
  SunGlobal = Cast<ADirectionalLight> (UGameplayStatics::GetActorOfClass (GetWorld (), ADirectionalLight::StaticClass ()));
  if (!SunGlobal) {
    UE_LOG (LogTemp, Warning, TEXT ("Unable to initialize SunGlobal"))
  }
}

EEightDir UEightDirActorComponent::GetFlipbookDirection (FRotator ControlRotation, FRotator ComponentRotation) {
  return GetDirection (UKismetMathLibrary::NormalizedDeltaRotator (ComponentRotation, ControlRotation).Yaw);
}

void UEightDirActorComponent::UpdateDisplayFlipbook (bool ForceUpdate, float SpeedOverride) {

  if (DisplayFlipbook && (ForceUpdate || DisplayFlipbook->WasRecentlyRendered (0.1f))) {
    FRotator ControlRotation;
    EEightDir DisplayFlipbookDirection = EEightDir::North;

    if (!CameraManagerGlobal) {
      CameraManagerGlobal = GetWorld ()->GetFirstPlayerController ()->PlayerCameraManager;
    }

    // BEEBE TODO: Remove?
    if (CameraManagerGlobal) {
      // Rotate the display flipbook
      ControlRotation = CameraManagerGlobal->GetCameraRotation ();
      DisplayFlipbook->SetWorldRotation (FRotator (0.0f, ControlRotation.Yaw + FLIPBOOK_ROTATIONAL_OFFSET, 0.0f));
      UpdateFlipbook (
        GetFlipbookDirection (ControlRotation, RootComponentGlobal->GetComponentRotation ()),
        EEightDir::EightDirMax,
        SpeedOverride >= 0.0f ? SpeedOverride : PrimitiveRootComponentGlobal->GetPhysicsLinearVelocity ().Size ()
      );

    }
  }
}

void UEightDirActorComponent::UpdateShadowFlipbook (bool ForceUpdate, float SpeedOverride) {

  if (ShadowFlipbook && (ForceUpdate || ShadowFlipbook->WasRecentlyRendered (0.1f))) {
    FRotator ControlRotation;
    EEightDir ShadowFlipbookDirection = EEightDir::North;

    if (bCastShadowGlobal) {

      // BEEBE TODO: Remove?
      if (!SunGlobal) {
        SunGlobal = Cast<ADirectionalLight> (UGameplayStatics::GetActorOfClass (GetWorld (), ADirectionalLight::StaticClass ()));
      }

      if (SunGlobal) {
        // Rotate the shadow flipbook
        ControlRotation = SunGlobal->GetActorRotation ();
        ShadowFlipbook->SetWorldRotation (FRotator (0.0f, ControlRotation.Yaw + FLIPBOOK_ROTATIONAL_OFFSET, 0.0f));
        UpdateFlipbook (
          EEightDir::EightDirMax,
          GetFlipbookDirection (ControlRotation, RootComponentGlobal->GetComponentRotation ()),
          SpeedOverride >= 0.0f ? SpeedOverride : PrimitiveRootComponentGlobal->GetPhysicsLinearVelocity ().Size ()
        );
      }

      // Test rotating the sun
      //SunGlobal->SetActorRotation(FRotator(ControlRotation.Pitch, ControlRotation.Yaw + 1.0f, ControlRotation.Roll));
    }
  }
}

void UEightDirActorComponent::UpdateDisplayAndShadowFlipbooks (bool ForceUpdate, float SpeedOverride) {
  if (DisplayFlipbook && (ForceUpdate || DisplayFlipbook->WasRecentlyRendered (0.1f))) {
    EEightDir ShadowFlipbookDirection = EEightDir::North;
    EEightDir DisplayFlipbookDirection = EEightDir::North;
    FRotator ControlRotation;

    // BEEBE TODO: Remove?
    if (!CameraManagerGlobal) {
      CameraManagerGlobal = GetWorld ()->GetFirstPlayerController ()->PlayerCameraManager;
    }

    if (CameraManagerGlobal) {
      // Rotate the display flipbook
      ControlRotation = CameraManagerGlobal->GetCameraRotation ();
      DisplayFlipbook->SetWorldRotation (FRotator (0.0f, ControlRotation.Yaw + FLIPBOOK_ROTATIONAL_OFFSET, 0.0f));
      DisplayFlipbookDirection = GetFlipbookDirection (ControlRotation, RootComponentGlobal->GetComponentRotation ());
    }

    if (ShadowFlipbook && bCastShadowGlobal) {
      // BEEBE TODO: Remove?
      if (!SunGlobal) {
        SunGlobal = Cast<ADirectionalLight> (UGameplayStatics::GetActorOfClass (GetWorld (), ADirectionalLight::StaticClass ()));
      }

      if (SunGlobal) {
        // Rotate the shadow flipbook
        ControlRotation = SunGlobal->GetActorRotation ();
        ShadowFlipbook->SetWorldRotation (FRotator (0.0f, ControlRotation.Yaw + FLIPBOOK_ROTATIONAL_OFFSET, 0.0f));
        ShadowFlipbookDirection = GetFlipbookDirection (ControlRotation, RootComponentGlobal->GetComponentRotation ());
      }
    }

    // Only update the flipbook(s) if the component has been rendered recently
    UpdateFlipbook (
      DisplayFlipbookDirection,
      ShadowFlipbookDirection,
      SpeedOverride >= 0.0f ? SpeedOverride : PrimitiveRootComponentGlobal->GetPhysicsLinearVelocity ().Size ()
    );
  }
}
