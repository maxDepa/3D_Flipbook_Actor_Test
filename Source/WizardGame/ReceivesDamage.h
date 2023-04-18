//
// Copyright (C) Taylor Beebe
//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ReceivesDamage.generated.h"

UINTERFACE (MinimalAPI)
class UReceivesDamage : public UInterface {
  GENERATED_BODY ()
};

class WIZARDGAME_API IReceivesDamage {
  GENERATED_BODY ()

public:

  /**
    Returns if the actor can be aimed at

    @retval true  - This actor can be aimed at
    @retval false - This actor can not be aimed at
  **/
  UFUNCTION (BlueprintNativeEvent, BlueprintCallable, Category = "Receives Damage Interface")
    bool IsAimEnabled () const;

  /**
    Enables or disables aiming at this actor

    @param Enabled - If true, this actor can be aimed at, otherwise this actor cannot be aimed at.
  **/
  UFUNCTION (BlueprintNativeEvent, BlueprintCallable, Category = "Receives Damage Interface")
    void SetAimEnabled (bool Enabled);

  /**
    Returns if the actor is currently dead

    @retval true  - This actor is dead
    @retval false - This actor is not dead
  **/
  UFUNCTION (BlueprintNativeEvent, BlueprintCallable, Category = "Receives Damage Interface")
    bool IsDead () const;

  /**
    Called when the actor has died
  **/
  UFUNCTION (BlueprintNativeEvent, BlueprintCallable, Category = "Receives Damage Interface")
    void OnDeath ();

  /**
    Returns if the actor can currently be damaged

    @retval true  - This actor can be damaged
    @retval false - This actor can not be damaged
  **/
  UFUNCTION (BlueprintNativeEvent, BlueprintCallable, Category = "Receives Damage Interface")
    bool CanBeDamaged () const;

  /**
    Sets whether or not this actor can be damaged

    @param Enabled - If true, this actor can be damaged. If false, this actor can not be damaged
  **/
  UFUNCTION (BlueprintNativeEvent, BlueprintCallable, Category = "Receives Damage Interface")
    void SetCanBeDamaged (bool Enabled);

  /**
    Applies damage to the actor

    @param DamageAmount - The amount of damage to apply
    @param DamageInflicter - The actor that is causing the damage
    @param Instigator - The controller that is causing the damage
    @param HitLocation - The location of the hit
    @param HitNormal - The normal of the hit
  **/
  UFUNCTION (BlueprintNativeEvent, BlueprintCallable, Category = "Receives Damage Interface")
    void ReceiveDamage (
    float DamageAmount,
    AActor *DamageInflicter,
    AController *Instigator,
    FVector HitLocation,
    FVector HitNormal
    );

  /**
    Gets the current health of the implementing actor

    @return The current health of the actor
  **/
  UFUNCTION (BlueprintNativeEvent, Category = "Receives Damage Interface")
    float GetCurrentHealth () const;

  /**
    Gets the maximum health of the implementing actor

    @return The maximum health of the actor
  **/
  UFUNCTION (BlueprintNativeEvent, Category = "Receives Damage Interface")
    float GetMaxHealth () const;

  /**
    Sets the material to apply to the implementing actor when it is being targeted

    @param Material - The material to apply
  **/
  UFUNCTION (BlueprintNativeEvent, Category = "Receives Damage Interface")
    void SetTargetedMaterial (
    UMaterialInterface *Material
    );

  /**
    Applies the targeted material to the implementing actor

    @param bTargeted - If true, the targeted material will be applied, otherwise
                       the targeted material will be removed.
  **/
  UFUNCTION (BlueprintNativeEvent, Category = "Receives Damage Interface")
    void ApplyTargetedMaterial (bool bTargeted);

  /**
    Clears the targeted material from the implementing actor
  **/
  UFUNCTION (BlueprintNativeEvent, Category = "Receives Damage Interface")
    void ClearTargetedMaterial ();
};