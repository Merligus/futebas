// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResultadoData.h"
#include "UObject/NoExportTypes.h"
#include "PartidasLinhaData.generated.h"

/**
 * 
 */
// UCLASS()
// class OJOGO_API UPartidasLinhaData : public UObject
// {
// 	GENERATED_BODY()
	
// };

USTRUCT(BlueprintType)
struct FPartidasLinhaData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PartidasLinha")
	TMap<int32, FResultadoData> fora;

	void AddNewColumn(int32 index_time);

	FPartidasLinhaData(){}
};