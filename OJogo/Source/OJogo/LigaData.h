// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CampeonatoData.h"
#include "UObject/NoExportTypes.h"
#include "LigaData.generated.h"

/**
 * 
 */
// UCLASS()
// class OJOGO_API ULigaData : public UObject
// {
// 	GENERATED_BODY()
	
// };

USTRUCT(BlueprintType)
struct FLigaData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
    TMap<int32, int32> teamMap;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCampeonatoData> tabelas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 divisoes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 times_por_divisao;

	FLigaData();

	FLigaData(int32 n_times_por_divisao, int32 n_times);

	void alocarTimes();

	int32 putTeamAsLast(int32 true_index);

	void bindIndexTimeDivisoes();

	void terminaTemporada();
};