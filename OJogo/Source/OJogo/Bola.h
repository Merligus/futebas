// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundWave.h"
#include "Net/UnrealNetwork.h"
#include "Bola.generated.h"

UCLASS()
class OJOGO_API ABola : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Bola", Replicated)
	USphereComponent* esfera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Bola", Replicated)
	UStaticMeshComponent* aparencia;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations, Replicated)
	UPaperFlipbookComponent* explosao;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sons)
	USoundWave* som_chute;

	// Sets default values for this actor's properties
	ABola();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void explode(FVector posicao);
	void explode_Implementation(FVector posicao);

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void fazSomChute(FVector posicao);
	void fazSomChute_Implementation(FVector posicao);

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void chuta(FRotator anguloChute, float forca);
	void chuta_Implementation(FRotator anguloChute, float forca);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void SV_chuta(FRotator anguloChute, float forca);
	void SV_chuta_Implementation(FRotator anguloChute, float forca);

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void MC_chuta(FRotator anguloChute, float forca);
	void MC_chuta_Implementation(FRotator anguloChute, float forca);
};
