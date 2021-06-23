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
	float MaxForcaChute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float MaxForcaCabeceio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float velocidade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float velocidadeCarrinho;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float aceleracaoCarrinho;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float staminaRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float jumpStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
	float slidingStaminaCost;

	FHabilidadesData()
	{
		MaxForcaChute = 5000.0f;
		MaxForcaCabeceio = 5000.0f;
		velocidade = 600.0f;
		velocidadeCarrinho = 1200.0f;
		aceleracaoCarrinho = 10000.0f;
		staminaRegen = 0.05f;
		jumpStaminaCost = 5.0f;
		slidingStaminaCost = 20.0f;
	}

	~FHabilidadesData()
	{

	}
};