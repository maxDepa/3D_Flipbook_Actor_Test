//
// Copyright (C) Taylor Beebe
//

#include "FlipbookActor.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"

// AFlipbookActor constructor
AFlipbookActor::AFlipbookActor()
{
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
  DisplayFlipbook->ComponentTags = {TEXT ("DisplayFlipbook")};

  // Setup the shadow flipbook
  ShadowFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent> (TEXT ("EightDirShadowFlipbook"));
  ShadowFlipbook->SetupAttachment (RootComponent);
  ShadowFlipbook->SetRelativeScale3D (FVector (2.0f, 2.0f, 2.0f));
  ShadowFlipbook->SetRelativeLocation (FVector (0.0f, 0.0f, -30.0f));
  ShadowFlipbook->bHiddenInGame = true;
  ShadowFlipbook->CastShadow = false;
  ShadowFlipbook->bCastHiddenShadow = true;
  ShadowFlipbook->ComponentTags = {TEXT ("ShadowFlipbook")};

}

/**
  Run when the game starts or when spawned
**/
void AFlipbookActor::BeginPlay()
{
	Super::BeginPlay();
}

/**
  Called every frame

  @param DeltaTime: The time since the last frame
**/
void AFlipbookActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

