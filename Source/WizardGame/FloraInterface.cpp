// Fill out your copyright notice in the Description page of Project Settings.


#include "FloraInterface.h"

void IFloraInterface::Grow () {

  EFloraState State = GetFloraState ();

  if (State < EFloraState::Mature) {
    SetFloraState (static_cast<EFloraState>(static_cast<uint8>(State) + 1));
  }
}

void IFloraInterface::Shrink () {
  EFloraState State = GetFloraState ();

  if (State > EFloraState::Seedling) {
    SetFloraState (static_cast<EFloraState>(static_cast<uint8>(State) - 1));
  }
}

void IFloraInterface::Destroy () {
  SetFloraState (EFloraState::Destroyed);
}
