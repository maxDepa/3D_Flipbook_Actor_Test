// Fill out your copyright notice in the Description page of Project Settings.


#include "Herb.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"

UEightDirActorComponent *CurrentStateComponentGlobal;

AHerb::AHerb () {
  SetFloraState (EFloraState::Seedling);
  PrimaryActorTick.bCanEverTick = true;
  CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent> (TEXT ("Root Component"));
  CapsuleComponent->InitCapsuleSize (34.0f, 88.0f);
  CapsuleComponent->SetCollisionProfileName (UCollisionProfile::Pawn_ProfileName);

  CapsuleComponent->CanCharacterStepUpOn = ECB_No;
  CapsuleComponent->SetShouldUpdatePhysicsVolume (true);
  CapsuleComponent->SetCanEverAffectNavigation (false);
  CapsuleComponent->bDynamicObstacle = true;
  RootComponent = CapsuleComponent;

  // Setup the display flipbook
  DisplayFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent> (TEXT ("EightDirDisplayFlipbook"));
  DisplayFlipbook->SetupAttachment (RootComponent);
  DisplayFlipbook->CastShadow = false;
  DisplayFlipbook->SetRelativeScale3D (FVector (2.0f, 2.0f, 2.0f));
  DisplayFlipbook->SetRelativeLocation (FVector (0.0f, 0.0f, -30.0f));

  // Setup the shadow flipbook
  ShadowFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent> (TEXT ("EightDirShadowFlipbook"));
  ShadowFlipbook->SetupAttachment (RootComponent);
  ShadowFlipbook->SetRelativeScale3D (FVector (2.0f, 2.0f, 2.0f));
  ShadowFlipbook->SetRelativeLocation (FVector (0.0f, 0.0f, -30.0f));
  ShadowFlipbook->bHiddenInGame = true;
  ShadowFlipbook->CastShadow = false;
  ShadowFlipbook->bCastHiddenShadow = true;

  SeedlingActorComponent = CreateDefaultSubobject<UEightDirActorComponent> (TEXT ("SeedlingActorComponent"));
  SeedlingActorComponent->LoadFlipbooksFromDirectory (TEXT ("/Game/Taylors_Folder/PixelArt/PlantPixelArt/GreenHerb/Seedling/"), true, false, false, true);
  SeedlingActorComponent->SetupAttachment (RootComponent, DisplayFlipbook, ShadowFlipbook, true);

  SaplingActorComponent = CreateDefaultSubobject<UEightDirActorComponent> (TEXT ("SaplingActorComponent"));
  SaplingActorComponent->LoadFlipbooksFromDirectory (TEXT ("/Game/Taylors_Folder/PixelArt/PlantPixelArt/GreenHerb/Sapling/"), true, false, false, true);
  SaplingActorComponent->SetupAttachment (RootComponent, DisplayFlipbook, ShadowFlipbook, true);

  YoungActorComponent = CreateDefaultSubobject<UEightDirActorComponent> (TEXT ("YoungActorComponent"));
  YoungActorComponent->LoadFlipbooksFromDirectory (TEXT ("/Game/Taylors_Folder/PixelArt/PlantPixelArt/GreenHerb/Young/"), true, false, false, true);
  YoungActorComponent->SetupAttachment (RootComponent, DisplayFlipbook, ShadowFlipbook, true);

  MatureActorComponent = CreateDefaultSubobject<UEightDirActorComponent> (TEXT ("MatureActorComponent"));
  MatureActorComponent->LoadFlipbooksFromDirectory (TEXT ("/Game/Taylors_Folder/PixelArt/PlantPixelArt/GreenHerb/Mature/"), true, false, false, true);
  MatureActorComponent->SetupAttachment (RootComponent, DisplayFlipbook, ShadowFlipbook, true);

  CurrentStateComponentGlobal = SeedlingActorComponent;
}

void AHerb::Tick (float DeltaTime) {
  Super::Tick (DeltaTime);
  CurrentStateComponentGlobal->UpdateDisplayAndShadowFlipbooks (true);
}

void AHerb::BeginPlay () {
  Super::BeginPlay ();
  CurrentStateComponentGlobal->UpdateDisplayAndShadowFlipbooks (true);
}

bool AHerb::RequiresWater () const {
  return bRequiresWater;
}

bool AHerb::IsEdible () const {
  return bIsEdible;
}

int AHerb::GetNumDaysToGrow () const {
  return NumDaysToGrow;
}

EFloraState AHerb::GetFloraState () const {
  return CurrentState;
}

void AHerb::SetFloraState (EFloraState NewState) {
  CurrentState = NewState;
  switch (CurrentState) {
    case EFloraState::Seedling:
      CurrentStateComponentGlobal = SeedlingActorComponent;
      break;
    case EFloraState::Sapling:
      CurrentStateComponentGlobal = SaplingActorComponent;
      break;
    case EFloraState::Young:
      CurrentStateComponentGlobal = YoungActorComponent;
      break;
    case EFloraState::Mature:
      CurrentStateComponentGlobal = MatureActorComponent;
      break;
    default:
      CurrentStateComponentGlobal = SeedlingActorComponent;
      break;
  }
}