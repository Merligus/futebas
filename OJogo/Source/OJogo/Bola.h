// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "OJogoGameState.h"
#include "Bola.generated.h"

UCLASS()
class OJOGO_API ABola : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Bola")
	USphereComponent* esfera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Bola")
	UStaticMeshComponent* aparencia;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bola")
	UNiagaraSystem* explosao_NS;

	// Sets default values for this actor's properties
	ABola();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void explode(FVector posicao);

	UFUNCTION(BlueprintCallable)
	void chuta(FRotator anguloChute, float forca);

};
