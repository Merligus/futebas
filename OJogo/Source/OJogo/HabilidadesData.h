// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HabilidadesData.generated.h"

/**
 * 
 */
// UCLASS()
// class OJOGO_API UHabilidadesData : public UObject
// {
// 	GENERATED_BODY()
	
// };

USTRUCT(BlueprintType)
struct FHabilidadesData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float maxForcaChute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float maxForcaCabeceio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float velocidade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float velocidadeCarrinho;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float staminaRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float jumpStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float slidingStaminaCost;

	FHabilidadesData()
	{
		maxForcaChute = 0.5f;
		maxForcaCabeceio = 0.5f;
		velocidade = 0.5f;
		velocidadeCarrinho = 0.5f;
		staminaRegen = 0.5f;
		jumpStaminaCost = 0.5f;
		slidingStaminaCost = 0.5f;
	}

	~FHabilidadesData()
	{

	}

	FORCEINLINE float converteMaxForcaChute() const { return maxForcaChute*(5000) + 3000; }
	FORCEINLINE float converteMaxForcaCabeceio() const { return maxForcaCabeceio*(5000) + 3000; }
	FORCEINLINE float converteVelocidade() const { return velocidade*(300) + 500; }
	FORCEINLINE float converteVelocidadeCarrinho() const { return velocidadeCarrinho*(600) + 1000; }
	FORCEINLINE float converteAceleracaoCarrinho() const { return velocidadeCarrinho*(6000) + 10000; }
	FORCEINLINE float converteStaminaRegen() const { return staminaRegen*(0.05) + 0.05; }
	FORCEINLINE float converteJumpStaminaCost() const { return jumpStaminaCost*(8) + 2; }
	FORCEINLINE float converteSlidingStaminaCost() const { return slidingStaminaCost*(15) + 5; }
	FORCEINLINE float overall() const { return (maxForcaChute + maxForcaCabeceio + velocidade + velocidadeCarrinho + staminaRegen)/5.0f; }

};