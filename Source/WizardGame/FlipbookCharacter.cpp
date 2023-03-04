// Fill out your copyright notice in the Description page of Project Settings.

#include "FlipbookCharacter.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"

// Example screen space debug string
// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Check it: %s"), *SomeString);

// Sets default values
AFlipbookCharacter::AFlipbookCharacter () {

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

  FlipbookCharacterMaxRunSpeed = 450.0f;
  FlipbookCharacterMaxWalkSpeed = 200.0f;
  GetCharacterMovement ()->MaxAcceleration = FLT_MAX;
  GetCharacterMovement ()->BrakingDecelerationWalking = FLT_MAX;
  GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, FLT_MAX);
  GetCharacterMovement ()->bOrientRotationToMovement = true;

  EightDirActorComponent = CreateDefaultSubobject<UEightDirActorComponent> (TEXT ("EightDirActorComponent"));
  EightDirActorComponent->SetupAttachment (TEXT ("/Game/PixelArt/CharacterPixelArt/Flipbooks"), true, true, true, true, false, true, FlipbookCharacterMaxWalkSpeed);

  static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputContextAsset (TEXT ("/Game/Input/IMC_TaylorCharacter.IMC_TaylorCharacter"));
  if (InputContextAsset.Succeeded ()) {
    InputMappingContext = InputContextAsset.Object;
  } else {
    UE_LOG (LogTemp, Warning, TEXT ("InputMappingContextAsset not found"));
  }

  static ConstructorHelpers::FObjectFinder<UInputAction> LookActionAsset (TEXT ("/Game/Input/Actions/IA_TaylorLook.IA_TaylorLook"));
  if (LookActionAsset.Succeeded ()) {
    LookInputAction = LookActionAsset.Object;
  } else {
    UE_LOG (LogTemp, Warning, TEXT ("LookActionAsset not found"));
  }

  static ConstructorHelpers::FObjectFinder<UInputAction> RunActionAsset (TEXT ("/Game/Input/Actions/IA_TaylorRun.IA_TaylorRun"));
  if (RunActionAsset.Succeeded ()) {
    RunInputAction = RunActionAsset.Object;
  } else {
    UE_LOG (LogTemp, Warning, TEXT ("RunActionAsset not found"));
  }

  static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionAsset (TEXT ("/Game/Input/Actions/IA_TaylorJump.IA_TaylorJump"));
  if (JumpActionAsset.Succeeded ()) {
    JumpInputAction = JumpActionAsset.Object;
  } else {
    UE_LOG (LogTemp, Warning, TEXT ("JumpActionAsset not found"));
  }

  static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionAsset (TEXT ("/Game/Input/Actions/IA_TaylorMove.IA_TaylorMove"));
  if (MoveActionAsset.Succeeded ()) {
    MoveInputAction = MoveActionAsset.Object;
  } else {
    UE_LOG (LogTemp, Warning, TEXT ("MoveActionAsset not found"));
  }

  static ConstructorHelpers::FObjectFinder<UInputAction> AimActionAsset (TEXT ("/Game/Input/Actions/IA_TaylorAim.IA_TaylorAim"));
  if (AimActionAsset.Succeeded ()) {
    AimInputAction = AimActionAsset.Object;
  } else {
    UE_LOG (LogTemp, Warning, TEXT ("AimActionAsset not found"));
  }

  PrimaryActorTick.bCanEverTick = true;

}

void AFlipbookCharacter::Look (const FInputActionValue &Value) {
  FVector2D LookAxisVector = Value.Get<FVector2D> ();

  if (!IsAiming) {
    if (Controller != nullptr) {
      AddControllerYawInput (LookAxisVector.X);
      AddControllerPitchInput (LookAxisVector.Y);
    }
  } else {

    if (LookAxisVector.Size () < 0.5f) {
      return;
    }

    // Convert the 2D vector to a unit vector
    FVector2D UnitVector = LookAxisVector.GetSafeNormal ();

    // Get the angle of the vector
    float Yaw = FMath::RadiansToDegrees (FMath::Atan2 (UnitVector.Y, UnitVector.X));

    Yaw += LocalCameraManager->GetCameraRotation ().Yaw;

    // Normalize Yaw to be between -180 and 180 in the same orientation as GetDirection() expects
    Yaw = FMath::Fmod ((Yaw + 90.0f) + 180.0f, 360.0f);
    if (Yaw < 0.0f) {
      Yaw += 360.0f;
    }
    Yaw -= 180.0f;

    RootComponent->SetWorldRotation (FRotator (0.0f, Yaw, 0.0f));
  }
  EightDirActorComponent->UpdateDisplayFlipbook (true, 0.0f);
}

void AFlipbookCharacter::Run (const FInputActionValue &Value) {
  GetCharacterMovement ()->MaxWalkSpeed = FlipbookCharacterMaxRunSpeed;
}

void AFlipbookCharacter::StopRunning (const FInputActionValue &Value) {
  GetCharacterMovement ()->MaxWalkSpeed = FlipbookCharacterMaxWalkSpeed;
}

void AFlipbookCharacter::Aim (const FInputActionValue &Value) {
  IsAiming = true;
  GetCharacterMovement ()->bOrientRotationToMovement = false;
}

void AFlipbookCharacter::StopAiming (const FInputActionValue &Value) {
  IsAiming = false;
  GetCharacterMovement ()->bOrientRotationToMovement = true;
}

void AFlipbookCharacter::Move (const FInputActionValue &Value) {
  const FVector2D Vector = Value.Get<FVector2D> ();

  if (Controller != nullptr) {

    if (LocalCameraManager == nullptr) {
      return;
    }

    // find out which way is forward
    const FRotator Rotation = Controller->GetControlRotation ();
    const FRotator YawRotation (0, Rotation.Yaw, 0);

    // get forward vector
    const FVector ForwardDirection = FRotationMatrix (YawRotation).GetUnitAxis (EAxis::X);

    // get right vector 
    const FVector RightDirection = FRotationMatrix (YawRotation).GetUnitAxis (EAxis::Y);

    // add movement 
    AddMovementInput (ForwardDirection, Vector.Y);
    AddMovementInput (RightDirection, Vector.X);
  }
  EightDirActorComponent->UpdateDisplayFlipbook ();
}

void AFlipbookCharacter::StopMoving (const FInputActionValue &Value) {
  EightDirActorComponent->UpdateDisplayFlipbook (true, 0.0f);
}

// Define a function to spawn an actor at a given location and rotation
void AFlipbookCharacter::SpawnActors () {

  // Get the world pointer
  UWorld *World = GetWorld ();

  // Get the player location
  FVector PlayerLocation = GetActorLocation ();

  // Define a constant for the number of actors to spawn
  const int NUM_ACTORS = 5;

  // Define a constant for the radius of the circle around the player
  const float RADIUS = 500.0f;

  // Loop through the number of actors to spawn
  for (int i = 0; i < NUM_ACTORS; i++) {

    // Calculate the angle of the current actor in radians
    float Angle = i * 2 * PI / NUM_ACTORS;

    // Calculate the offset from the player location
    FVector Offset;
    Offset.X = RADIUS * cos (Angle);
    Offset.Y = RADIUS * sin (Angle);
    Offset.Z = 0.0f;

    // Calculate the spawn location
    FVector SpawnLocation = PlayerLocation + Offset;

    // Calculate the spawn rotation
    FRotator SpawnRotation;
    SpawnRotation.Yaw = Angle * 180 / PI;
    SpawnRotation.Pitch = 0.0f;
    SpawnRotation.Roll = 0.0f;

    // Check if the world is valid
    if (World) {
      // Define the spawn parameters
      FActorSpawnParameters SpawnParams;
      SpawnParams.Owner = this;
      SpawnParams.Instigator = GetInstigator ();

      UE_LOG (LogTemp, Warning, TEXT ("Spawning at: %s"), *SpawnLocation.ToString ());

      // Spawn the actor
      AActor *SpawnedActor = World->SpawnActor<AActor> (TestActorClass, SpawnLocation, SpawnRotation, SpawnParams);
    }
  }
}

// Called when the game starts or when spawned
void AFlipbookCharacter::BeginPlay () {
  Super::BeginPlay ();

  // Populate the LocalCameraManager global
  LocalCameraManager = GetWorld ()->GetFirstPlayerController ()->PlayerCameraManager;
  if (!LocalCameraManager) {
    UE_LOG (LogTemp, Warning, TEXT ("Unable to initialize LocalCameraManager"));
  }

  if (APlayerController *PlayerController = Cast<APlayerController> (GetController ())) {
    if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> (PlayerController->GetLocalPlayer ())) {
      UE_LOG (LogTemp, Warning, TEXT ("Adding mapping context"));
      Subsystem->AddMappingContext (InputMappingContext, 0);
    }
  }

  GetCharacterMovement ()->MaxWalkSpeed = FlipbookCharacterMaxWalkSpeed;

  SpawnActors ();
  EightDirActorComponent->UpdateDisplayAndShadowFlipbooks (true);
}

// Called every frame
void AFlipbookCharacter::Tick (float DeltaTime) {
  Super::Tick (DeltaTime);

  EightDirActorComponent->UpdateShadowFlipbook (true);

  if (IsAiming) {

    // Test with capsule trace
    //
    FVector Start = GetActorLocation ();
    FVector End = Start + RootComponent->GetComponentRotation ().Vector () * 100000.0f;
    float Radius = 50.0f;
    float HalfAngle = FMath::DegreesToRadians (30.0f);
    float Length = Radius / FMath::Tan (HalfAngle);
    float HalfHeight = Length / 2.0f;

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add (UEngineTypes::ConvertToObjectType (ECollisionChannel::ECC_WorldStatic));
    ObjectTypes.Add (UEngineTypes::ConvertToObjectType (ECollisionChannel::ECC_WorldDynamic));

    TArray<FHitResult> HitResults;
    bool bHit = UKismetSystemLibrary::CapsuleTraceMultiForObjects (
      GetWorld (),
      Start,
      End,
      Radius,
      HalfHeight,
      ObjectTypes,
      false,
      {},
      EDrawDebugTrace::ForDuration,
      HitResults,
      false,
      FLinearColor::Red,
      FLinearColor::Red,
      0.1f
    );

    // Test with sphere trace
    //
    //float Radius = 50.0f;
    //float HalfAngle = FMath::DegreesToRadians (30.0f);
    //// Create an array of object types to trace
    //TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    //ObjectTypes.Add (UEngineTypes::ConvertToObjectType (ECollisionChannel::ECC_WorldStatic));
    //ObjectTypes.Add (UEngineTypes::ConvertToObjectType (ECollisionChannel::ECC_WorldDynamic));

    //// Create an array to store the hit results
    //TArray<FHitResult> OutHits;

    //TArray<AActor *> ActorsToIgnore;

    //TArray<FHitResult> HitResults;
    //// Perform the sphere trace
    //bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects (
    //	GetWorld (),
    //	Start,
    //	End,
    //	Radius,
    //	ObjectTypes,
    //	true,
    //	ActorsToIgnore,
    //	EDrawDebugTrace::ForDuration,
    //	OutHits,
    //	true
    //);

    // Test with line trace
    // 
    //DrawDebugLine (GetWorld (), Start, End, FColor::Red, false, -1.0f, 0, 5.0f);
  }
}

// Called to bind functionality to input
void AFlipbookCharacter::SetupPlayerInputComponent (UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent (PlayerInputComponent);

  // Set up action bindings
  if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent> (PlayerInputComponent)) {

    // Jumping
    EnhancedInputComponent->BindAction (JumpInputAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
    EnhancedInputComponent->BindAction (JumpInputAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

    // Moving
    EnhancedInputComponent->BindAction (MoveInputAction, ETriggerEvent::Triggered, this, &AFlipbookCharacter::Move);
    EnhancedInputComponent->BindAction (MoveInputAction, ETriggerEvent::Completed, this, &AFlipbookCharacter::StopMoving);

    // Looking
    EnhancedInputComponent->BindAction (LookInputAction, ETriggerEvent::Triggered, this, &AFlipbookCharacter::Look);

    // Running
    EnhancedInputComponent->BindAction (RunInputAction, ETriggerEvent::Triggered, this, &AFlipbookCharacter::Run);
    EnhancedInputComponent->BindAction (RunInputAction, ETriggerEvent::Completed, this, &AFlipbookCharacter::StopRunning);

    // Aiming
    EnhancedInputComponent->BindAction (AimInputAction, ETriggerEvent::Triggered, this, &AFlipbookCharacter::Aim);
    EnhancedInputComponent->BindAction (AimInputAction, ETriggerEvent::Completed, this, &AFlipbookCharacter::StopAiming);
  }
}
