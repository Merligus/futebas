// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartidasData.h"
#include "PosicaoData.h"
#include "ResultadoData.h"
#include "UObject/NoExportTypes.h"
#include "CampeonatoData.generated.h"

/**
 * 
 */
// UCLASS()
// class OJOGO_API UCampeonatoData : public UObject
// {
// 	GENERATED_BODY()
	
// };

USTRUCT(BlueprintType)
struct FCampeonatoData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Campeonato")
	TArray<FPosicaoData> tabela; // [n_times]

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Campeonato")
	FPartidasData confrontos; // [n_times x n_times]

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Campeonato")
	int32 n_times;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Campeonato")
	bool apenasIda;

	void atualizaTabela(int32 indexX, int32 indexY, FResultadoData resultado);

	void ordenaTabela();

	FCampeonatoData(){}
	FCampeonatoData(int32 n_times);
	FCampeonatoData(int32 n_times, bool apenasIda);
};