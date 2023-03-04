// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UPaperFlipbookComponent *DisplayFlipbook;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
		UPaperFlipbookComponent *ShadowFlipbook;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = "Root Component")
		UCapsuleComponent *CapsuleComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
