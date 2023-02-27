// Fill out your copyright notice in the Description page of Project Settings.


#include "FourDirActorComponent.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Engine/ObjectLibrary.h"

// Sets default values for this component's properties
UFourDirActorComponent::UFourDirActorComponent () {
  PrimaryComponentTick.bCanEverTick = false;

  // Set all flipbooks to the default
  static ConstructorHelpers::FObjectFinder<UPaperFlipbook> DefaultFlipbook (DEFAULT_FLIPBOOK_PATH);
  if (DefaultFlipbook.Succeeded ()) {
    StationaryNorthFlipbook = DefaultFlipbook.Object;
    StationaryEastFlipbook = DefaultFlipbook.Object;
    StationarySouthFlipbook = DefaultFlipbook.Object;
    StationaryWestFlipbook = DefaultFlipbook.Object;
    SlowNorthFlipbook = DefaultFlipbook.Object;
    SlowEastFlipbook = DefaultFlipbook.Object;
    SlowSouthFlipbook = DefaultFlipbook.Object;
    SlowWestFlipbook = DefaultFlipbook.Object;
    FastNorthFlipbook = DefaultFlipbook.Object;
    FastEastFlipbook = DefaultFlipbook.Object;
    FastSouthFlipbook = DefaultFlipbook.Object;
    FastWestFlipbook = DefaultFlipbook.Object;
  } else {
    UE_LOG (LogTemp, Warning, TEXT ("DefaultFlipbook not found"));
  }
}

void UFourDirActorComponent::LoadFlipbooksFromDirectory (const FString &Directory, bool stationary, bool slow, bool fast, bool TwoFlipbookRotation) {

  bStationaryGlobal = stationary;
  bSlowGlobal = slow;
  bFastGlobal = fast;

  if (!TwoFlipbookRotation) {
    if (slow) {
      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> NorthAsset (*FString::Printf (TEXT ("%s/slow_north.slow_north"), *Directory));
      if (NorthAsset.Succeeded ()) {
        SlowNorthFlipbook = NorthAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("NorthAsset not found"));
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
    }

    if (fast) {
      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FastNorthAsset (*FString::Printf (TEXT ("%s/fast_north.fast_north"), *Directory));
      if (FastNorthAsset.Succeeded ()) {
        FastNorthFlipbook = FastNorthAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("FastNorthAsset not found"));
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
    }

    if (stationary) {
      static ConstructorHelpers::FObjectFinder<UPaperFlipbook> StationaryNorthAsset (*FString::Printf (TEXT ("%s/stationary_north.stationary_north"), *Directory));
      if (StationaryNorthAsset.Succeeded ()) {
        StationaryNorthFlipbook = StationaryNorthAsset.Object;
      } else {
        UE_LOG (LogTemp, Warning, TEXT ("StationaryNorthAsset not found"));
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
      StationarySouthFlipbook = Flipbook1;
    } else {
      UE_LOG (LogTemp, Warning, TEXT ("File %s is not valid!"), *AssetDataList[0].PackageName.ToString ());
    }

    if (Flipbook2) {
      StationaryEastFlipbook = Flipbook2;
      StationaryWestFlipbook = Flipbook2;
    } else {
      UE_LOG (LogTemp, Warning, TEXT ("File %s is not valid!"), *AssetDataList[1].PackageName.ToString ());
    }
  }
}

void UFourDirActorComponent::UpdateFlipbook (UPaperFlipbookComponent *FlipbookToUpdate, EFourDir Direction, float SlowSpeed, float Speed) {

  if (!FlipbookToUpdate) {
    UE_LOG (LogTemp, Warning, TEXT ("FlipbookToUpdate is null"));
    return;
  }

  UPaperFlipbook *FlipbookToUse = nullptr;

  if (Speed >= SlowSpeed + 10.0f && bFastGlobal) {
    switch (Direction) {
      case EFourDir::North:
        FlipbookToUse = FastNorthFlipbook;
        break;
      case EFourDir::East:
        FlipbookToUse = FastEastFlipbook;
        break;
      case EFourDir::South:
        FlipbookToUse = FastSouthFlipbook;
        break;
      case EFourDir::West:
        FlipbookToUse = FastWestFlipbook;
        break;
    }
  } else if (Speed >= 1.0f && bSlowGlobal) {
    switch (Direction) {
      case EFourDir::North:
        FlipbookToUse = SlowNorthFlipbook;
        break;
      case EFourDir::East:
        FlipbookToUse = SlowEastFlipbook;
        break;
      case EFourDir::South:
        FlipbookToUse = SlowSouthFlipbook;
        break;
      case EFourDir::West:
        FlipbookToUse = SlowWestFlipbook;
        break;
    }
  } else if (bStationaryGlobal) {
    switch (Direction) {
      case EFourDir::North:
        FlipbookToUse = StationaryNorthFlipbook;
        break;
      case EFourDir::East:
        FlipbookToUse = StationaryEastFlipbook;
        break;
      case EFourDir::South:
        FlipbookToUse = StationarySouthFlipbook;
        break;
      case EFourDir::West:
        FlipbookToUse = StationaryWestFlipbook;
        break;
    }
  }

  if (!FlipbookToUse) {
    UE_LOG (LogTemp, Warning, TEXT ("Unable to determine correct flipbook!"));
    return;
  }

  FlipbookToUpdate->SetFlipbook (FlipbookToUse);
}
