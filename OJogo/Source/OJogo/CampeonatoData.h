// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartidasData.h"
#include "PosicaoData.h"
#include "ResultadoData.h"
#include "Templates/Tuple.h"
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
struct FJogoData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jogo")
	int32 casa;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jogo")
	int32 fora;

	FJogoData(){}
	FJogoData(int32 t1, int32 t2)
	{
		casa = t1;
		fora = t2;
	}
};

USTRUCT(BlueprintType)
struct FRodadaData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rodada")
	TArray<FJogoData> jogos;

	FRodadaData(){}
};

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
	TArray<FRodadaData> calendario; // [[(1, 2), (3, 4)],
								//  [(1, 3), (2, 4)],
								//  [(1, 4), (2, 3)]]

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Campeonato")
	TArray<int32> times; // {index_time1, index_time2, ..., index_timeN}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Campeonato")
	int32 n_times;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Campeonato")
	int32 rodada_atual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Campeonato")
	bool apenasIda;

	void adicionaTime(int32 index_time, int32 posicaoInicial);

	void montaConfrontos();

	void atualizaTabela(int32 index_casa, int32 index_fora, int32 gols_casa, int32 gols_fora);

	void terminaRodada();

	FCampeonatoData(){}
	FCampeonatoData(int32 n_times);
	FCampeonatoData(int32 n_times, bool apenasIda);
};