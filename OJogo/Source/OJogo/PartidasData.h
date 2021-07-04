// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartidasLinhaData.h"
#include "UObject/NoExportTypes.h"
#include "PartidasData.generated.h"

/**
 * 
 */
// UCLASS()
// class OJOGO_API UPartidasData : public UObject
// {
// 	GENERATED_BODY()
	
// };

USTRUCT(BlueprintType)
struct FPartidasData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Partidas")
	TMap<int32, FPartidasLinhaData> casa;

	void AddNewRow(int32 index_time);
	void AddUninitialized(TArray<int32> times);

	FPartidasData(){}
};