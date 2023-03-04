// Fill out your copyright notice in the Description page of Project Settings.


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

// Called when the game starts or when spawned
void AFlora::BeginPlay()
{
  Super::BeginPlay ();
  CurrentStateComponentGlobal->UpdateDisplayAndShadowFlipbooks (true);
}

bool AFlora::RequiresWater () {
  return bRequiresWater;
}

bool AFlora::IsEdible () {
  return bIsEdible;
}

int AFlora::GetNumDaysToGrow () {
  return bIsEdible;
}

EFloraState AFlora::GetFloraState () {
  return CurrentState;
}

void AFlora::SetFloraState (EFloraState NewState) {
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

void AFlora::Grow () {
  EFloraState State = GetFloraState ();

  if (State < EFloraState::Mature) {
    SetFloraState (static_cast<EFloraState>(static_cast<uint8>(State) + 1));
  }
}

void AFlora::Shrink () {
  EFloraState State = GetFloraState ();

  if (State > EFloraState::Seedling) {
    SetFloraState (static_cast<EFloraState>(static_cast<uint8>(State) - 1));
  }
}

void AFlora::Destroy () {
  SetFloraState (EFloraState::Destroyed);
}

void AFlora::Tick(float DeltaTime)
{
  Super::Tick (DeltaTime);
  CurrentStateComponentGlobal->UpdateDisplayAndShadowFlipbooks (true);
}
