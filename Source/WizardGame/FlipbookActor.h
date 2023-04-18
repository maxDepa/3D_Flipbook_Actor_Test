//
// Copyright (C) Taylor Beebe
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlipbookActor.generated.h"

class UCapsuleComponent;
class UPaperFlipbookComponent;

UCLASS()
class WIZARDGAME_API AFlipbookActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlipbookActor();

protected:
	// The display flipbook for the actor
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UPaperFlipbookComponent *DisplayFlipbook;

	// The shadow flipbook for the actor
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UPaperFlipbookComponent *ShadowFlipbook;

	// The root and capsule component for the actor
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Root Component")
		UCapsuleComponent *CapsuleComponent;

	/**
		Run when the game starts or when spawned
	**/
	virtual void BeginPlay() override;

public:	
	/**
		Called every frame

		@param DeltaTime: The time since the last frame
	**/	
	virtual void Tick(float DeltaTime) override;

};
