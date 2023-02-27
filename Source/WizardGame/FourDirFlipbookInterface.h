// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FourDirFlipbookInterface.generated.h"

UENUM (BlueprintType)
enum class EFourDir : uint8 {
	North,
	East,
	South,
	West
};

const FString DirectionStrings[] = {
	TEXT ("North"),
	TEXT ("East"),
	TEXT ("South"),
	TEXT ("West")
};

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

#define IS_NORTH(Yaw) (Yaw >= -45.0f && Yaw < 45.0f)
#define IS_EAST(Yaw) (Yaw >= 45.0f && Yaw <= 135.0f)
#define IS_SOUTH(Yaw) (Yaw > 135.0 || Yaw <= -135.0)
#define IS_WEST(Yaw) (Yaw > -135.0 && Yaw <= -45.0)

#define DEFAULT_FLIPBOOK_PATH TEXT ("/Game/Flipbooks/DefaultFlipbook/DefaultFlipbook.DefaultFlipbook")

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFourDirFlipbookInterface : public UInterface
{
	GENERATED_BODY()
};

class WIZARDGAME_API IFourDirFlipbookInterface
{
	GENERATED_BODY()

public:
	virtual EFourDir GetDirection (float Delta);
};
