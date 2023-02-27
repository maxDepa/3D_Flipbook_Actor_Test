// Fill out your copyright notice in the Description page of Project Settings.


#include "EightDirFlipbookInterface.h"

////
// COMPASS ORIENTATION (normalized degrees)
// 
// NW        N       NE
//    -45    0    45
// W  -90         90  E
//    -135  180  135
// SW        S       SE
// 
// NOTE TO SELF: Ensure any direction calculation for flipbooks is consistent
//				 with the above compass
////
EEightDir IEightDirFlipbookInterface::GetDirection (float Yaw) {

  if IS_NORTH (Yaw) {
    return EEightDir::North;
  } else if IS_NORTHEAST (Yaw) {
    return EEightDir::Northeast;
  } else if IS_EAST (Yaw) {
    return EEightDir::East;
  } else if IS_SOUTHEAST (Yaw) {
    return EEightDir::Southeast;
  } else if IS_SOUTH (Yaw) {
    return EEightDir::South;
  } else if IS_SOUTHWEST (Yaw) {
    return EEightDir::Southwest;
  } else if IS_WEST (Yaw) {
    return EEightDir::West;
  } else {
    return EEightDir::Northwest;
  }
}