// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PosicaoData.generated.h"

/**
 * 
 */
// UCLASS()
// class OJOGO_API UPosicaoData : public UObject
// {
// 	GENERATED_BODY()
	
// };

USTRUCT(BlueprintType)
struct FPosicaoData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posicao")
	int32 pontos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posicao")
	int32 partidas_jogadas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posicao")
	int32 vitorias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posicao")
	int32 empates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posicao")
	int32 derrotas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posicao")
	int32 gols_pro;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posicao")
	int32 gols_contra;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posicao")
	int32 saldo_de_gols;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posicao")
	int32 index_time;

	FPosicaoData();
	FPosicaoData(int32 index);
};
