// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResultadoData.generated.h"

/**
 * 
 */
// UCLASS()
// class OJOGO_API UResultadoData : public UObject
// {
// 	GENERATED_BODY()
	
// };

USTRUCT(BlueprintType)
struct FResultadoData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resultado")
	int32 gols_casa;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resultado")
	int32 gols_penalti_casa;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resultado")
	int32 index_casa;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resultado")
	int32 gols_fora;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resultado")
	int32 gols_penalti_fora;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resultado")
	int32 index_fora;

	FResultadoData();

	FResultadoData(int32 empate);

	FResultadoData(int32 casa, int32 fora);

	int32 getGanhador();
};