//
// Copyright (C) Taylor Beebe - All Rights Reserved Unauthorized copying of this repository,
// via any medium is strictly prohibited Proprietary and confidential 
// 
// Written by Taylor Beebe taylor.d.beebe@gmail.com, February 2023
//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AppliesDamage.generated.h"

UINTERFACE (MinimalAPI)
class UAppliesDamage : public UInterface {
  GENERATED_BODY ()
};

class WIZARDGAME_API IAppliesDamage {
  GENERATED_BODY ()

public:
  /**
    Function that applies damage to another actor.

    @param HitActor - The actor that was hit and will receive the damage.
    @param DamageAmount - The amount of damage to apply.
    @param DamageCauser - The actor that caused the damage.
    @param Instigator - The controller that caused the damage.
    @param HitLocation - The location where the hit occurred.
    @param HitNormal - The normal vector of the surface where the hit occurred.
  **/
  UFUNCTION (BlueprintNativeEvent, Category = "AppliesDamage")
    void ApplyDamage (
    AActor *HitActor,
    float DamageAmount,
    AActor *DamageCauser,
    AController *Instigator,
    FVector HitLocation,
    FVector HitNormal
    );

  /**
    Returns if this actor can currently deal damage to other actors.

    @retval true  - This actor can deal damage.
    @retval false - This actor can not deal damage.
  **/
  UFUNCTION (BlueprintNativeEvent, BlueprintCallable, Category = "Applies Damage")
    bool CanDealDamage () const;

  /**
    Sets if this actor can currently deal damage to other actors.

    @param bCanDealDamage - If true, this actor can deal damage. If false, this actor can not deal damage.
  **/
  UFUNCTION (BlueprintNativeEvent, BlueprintCallable, Category = "Applies Damage")
    void SetCanDealDamage (
    bool bCanDealDamage
    );

  /**
    Called after this actor deals damage to another actor.

    @param DamageAmount - The amount of damage that was dealt.
    @param DamagedActor - The actor that was damaged.
    @param DamageCauser - The actor that caused the damage.
    @param HitLocation - The location where the hit occurred.
    @param HitNormal - The normal vector of the surface where the hit occurred.
  **/
  UFUNCTION (BlueprintNativeEvent, BlueprintCallable, Category = "Applies Damage")
    void OnDealDamage (
    float DamageAmount,
    AActor *DamagedActor,
    AActor *DamageCauser,
    FVector HitLocation,
    FVector HitNormal
    );

};
