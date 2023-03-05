//
// Copyright (C) Taylor Beebe - All Rights Reserved Unauthorized copying of this repository,
// via any medium is strictly prohibited Proprietary and confidential 
// 
// Written by Taylor Beebe taylor.d.beebe@gmail.com, February 2023
//

#include "Flora.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "EightDirActorComponent.h"

UEightDirActorComponent *CurrentStateComponentGlobal;

// Sets default values
AFlora::AFlora()
{
  SetFloraState (EFloraState::Seedling);
  PrimaryActorTick.bCanEverTick = true;

  // Setup the root component
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
  DisplayFlipbook->ComponentTags = { TEXT ("DisplayFlipbook") };

  // Setup the shadow flipbook
  ShadowFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent> (TEXT ("EightDirShadowFlipbook"));
  ShadowFlipbook->SetupAttachment (RootComponent);
  ShadowFlipbook->SetRelativeScale3D (FVector (2.0f, 2.0f, 2.0f));
  ShadowFlipbook->SetRelativeLocation (FVector (0.0f, 0.0f, -30.0f));
  ShadowFlipbook->bHiddenInGame = true;
  ShadowFlipbook->CastShadow = false;
  ShadowFlipbook->bCastHiddenShadow = true;
  ShadowFlipbook->ComponentTags = { TEXT ("ShadowFlipbook") };

  SeedlingActorComponent = CreateDefaultSubobject<UEightDirActorComponent> (TEXT ("SeedlingActorComponent"));
  SaplingActorComponent = CreateDefaultSubobject<UEightDirActorComponent> (TEXT ("SaplingActorComponent"));
  YoungActorComponent = CreateDefaultSubobject<UEightDirActorComponent> (TEXT ("YoungActorComponent"));
  MatureActorComponent = CreateDefaultSubobject<UEightDirActorComponent> (TEXT ("MatureActorComponent"));
  
  CurrentStateComponentGlobal = SeedlingActorComponent;
}

/**
  Run when the game starts or when spawned
**/
void AFlora::BeginPlay()
{
  Super::BeginPlay ();
  CurrentStateComponentGlobal->UpdateDisplayAndShadowFlipbooks (true);
}

/**
  Returns if the flora instance requires water

  @retval true - The flora instance requires water
  @retval false - The flora instance does not require water
**/
bool AFlora::RequiresWater () {
  return bRequiresWater;
}

/**
  Returns if the flora instance can be eaten

  @retval true - The flora instance can be eaten
  @retval false - The flora instance cannot be eaten
**/
bool AFlora::IsEdible () {
  return bIsEdible;
}

/**
  Returns how many days the flora instance takes to grow from seedling to mature

  @retval int - The number of days to grow
**/
int AFlora::GetNumDaysToGrow () {
  return bIsEdible;
}

/**
  Get the current flora state enum value

  @retval EFloraState - The current flora state
**/
EFloraState AFlora::GetFloraState () {
  return CurrentState;
}

/**
  Set the current flora state to the input NewState

  @param NewState - The new flora state
**/
void AFlora::SetFloraState (EFloraState NewState) {

  CurrentState = NewState;

  // Set the current state flipbook component to the new state
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

/**
  Increment the flora state enum
**/
void AFlora::Grow () {
  EFloraState State = GetFloraState ();

  if (State < EFloraState::Mature) {
    SetFloraState (static_cast<EFloraState>(static_cast<uint8>(State) + 1));
  }
}

/**
  Decrement the flora state enum
**/
void AFlora::Shrink () {
  EFloraState State = GetFloraState ();

  if (State > EFloraState::Seedling) {
    SetFloraState (static_cast<EFloraState>(static_cast<uint8>(State) - 1));
  }
}

/**
  Destroy the flora state instance
**/
void AFlora::Destroy () {
  SetFloraState (EFloraState::Destroyed);
}

/**
  Called every frame

  @param DeltaTime: The time since the last frame
**/
void AFlora::Tick(float DeltaTime)
{
  Super::Tick (DeltaTime);
  CurrentStateComponentGlobal->UpdateDisplayAndShadowFlipbooks (true);
}
