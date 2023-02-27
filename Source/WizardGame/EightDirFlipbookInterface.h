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

#define STATIONARY_NORTH_INDEX 0
#define STATIONARY_NORTHEAST_INDEX 1
#define STATIONARY_EAST_INDEX 2
#define STATIONARY_SOUTHEAST_INDEX 3
#define STATIONARY_SOUTH_INDEX 4
#define STATIONARY_SOUTHWEST_INDEX 5
#define STATIONARY_WEST_INDEX 6
#define STATIONARY_NORTHWEST_INDEX 7
#define SLOW_NORTH_INDEX 8
#define SLOW_NORTHEAST_INDEX 9
#define SLOW_EAST_INDEX 10
#define SLOW_SOUTHEAST_INDEX 11
#define SLOW_SOUTH_INDEX 12
#define SLOW_SOUTHWEST_INDEX 13
#define SLOW_WEST_INDEX 14
#define SLOW_NORTHWEST_INDEX 15
#define FAST_NORTH_INDEX 16
#define FAST_NORTHEAST_INDEX 17
#define FAST_EAST_INDEX 18
#define FAST_SOUTHEAST_INDEX 19
#define FAST_SOUTH_INDEX 20
#define FAST_SOUTHWEST_INDEX 21
#define FAST_WEST_INDEX 22
#define FAST_NORTHWEST_INDEX 23
#define MAX_FLIPBOOK_INDEX 24

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
