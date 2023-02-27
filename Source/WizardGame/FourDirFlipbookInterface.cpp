// Fill out your copyright notice in the Description page of Project Settings.


#include "FourDirFlipbookInterface.h"

////
// COMPASS ORIENTATION (normalized degrees)
// 
//           N
//           0
// W  -90         90  E
//          180
//           S
// 
// NOTE: Ensure any direction calculation for flipbooks is consistent
//		 with the above compass
////
EFourDir IFourDirFlipbookInterface::GetDirection (float Yaw) {
  if IS_NORTH (Yaw) {
    return EFourDir::North;
  } else if IS_EAST (Yaw) {
    return EFourDir::East;
  } else if IS_SOUTH (Yaw) {
    return EFourDir::South;
  } else {
    return EFourDir::West;
  }
}