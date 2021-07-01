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
	TArray<FPartidasLinhaData> casa;

	void AddNewRow();
	void AddUninitialized(const int32 RowCount, const int32 ColCount);

	FPartidasData(){}
};