// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundWave.h"
#include "FireworkActor.generated.h"

UCLASS()
class OJOGO_API AFireworkActor : public APaperCharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireworkActor();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float velocity;

	UPROPERTY(BlueprintReadOnly)
	float index_rocket;

	UPROPERTY(BlueprintReadOnly)
	float index_explosion;

	UPROPERTY(BlueprintReadOnly)
	float rocket_timeout;

	UPROPERTY(BlueprintReadOnly)
	float explosion_timeout;

	UPROPERTY(BlueprintReadOnly)
	FVector start_position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UPaperFlipbook*> explosions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UPaperFlipbook*> rockets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* invisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sons)
	USoundWave* rocket_sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sons)
	USoundWave* explosion_sound;

public:

	void Fire();

	void StartAnimation();

	void UpdateAnimation();

	void EndAnimation();

};
