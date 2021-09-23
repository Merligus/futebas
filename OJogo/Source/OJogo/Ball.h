// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundWave.h"
#include "Net/UnrealNetwork.h"
#include "Ball.generated.h"

/**
 *
 */
UCLASS()
class OJOGO_API ABall : public AStaticMeshActor
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* explosao;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundWave* som_chute;

	// Sets default values for this actor's properties
	ABall();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void explode(FVector posicao);
	void explode_Implementation(FVector posicao);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void fazSomChute(FVector posicao);
	void fazSomChute_Implementation(FVector posicao);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void SV_chuta(FRotator anguloChute, float forca);
	void SV_chuta_Implementation(FRotator anguloChute, float forca);

};
