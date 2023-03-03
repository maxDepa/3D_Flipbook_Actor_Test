// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EightDirFlipbookInterface.generated.h"

////
// COMPASS ORIENTATION (normalized degrees)
// 
// NW        N       NE
//    -45    0    45
// W  -90         90  E
//    -135  180  135
// SW        S       SE
// 
// NOTE: Ensure any direction calculation for flipbooks is consistent
//		 with the above compass
////

UENUM (BlueprintType)
enum class EEightDir : uint8
{
	North,
	Northeast,
	East,
	Southeast,
	South,
	Southwest,
	West,
	Northwest,
	EightDirMax
};

const FString DirectionStrings[] = {
	TEXT ("North"),
	TEXT ("Northeast"),
	TEXT ("East"),
	TEXT ("Southeast"),
	TEXT ("South"),
	TEXT ("Southwest"),
	TEXT ("West"),
	TEXT ("Northwest"),
	TEXT ("EightDirMax")
};

enum class EEightDirFlipbookSpeeds : uint8 {
	Stationary,
  Slow,
  Fast,
  SpeedMax
};

const FString FlipbookSpeedStrings[] = {
  TEXT ("Stationary"),
  TEXT ("Slow"),
  TEXT ("Fast"),
  TEXT ("EightDirFlipbookTypeMax")
};

#define IS_NORTH(Yaw) (Yaw >= -22.5f && Yaw < 22.5f)
#define IS_NORTHEAST(Yaw) (Yaw >= 22.5f && Yaw < 67.5f)
#define IS_EAST(Yaw) (Yaw >= 67.5f && Yaw <= 112.5f)
#define IS_SOUTHEAST(Yaw) (Yaw > 112.5f && Yaw <= 157.5f)
#define IS_SOUTH(Yaw) (Yaw > 157.5f || Yaw <= -157.5f)
#define IS_SOUTHWEST(Yaw) (Yaw > -157.5f && Yaw <= -112.5f)
#define IS_WEST(Yaw) (Yaw > -112.5f && Yaw <= -67.5f)
#define IS_NORTHWEST(Yaw) (Yaw > -67.5f && Yaw <= -22.5f)

#define DEFAULT_FLIPBOOK_PATH TEXT ("/Game/Flipbooks/DefaultFlipbook/DefaultFlipbook.DefaultFlipbook")
#define DEFAULT_FLIPBOOK_SLOW_SPEED 150.0f

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UEightDirFlipbookInterface : public UInterface
{
	GENERATED_BODY()
};

class WIZARDGAME_API IEightDirFlipbookInterface
{
	GENERATED_BODY()

public:
	UFUNCTION (BlueprintCallable, Category = "Eight Dir Interface Functions")
		virtual EEightDir GetDirection (float Yaw);
};
