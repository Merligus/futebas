// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResultadoData.h"
#include "UObject/NoExportTypes.h"
#include "MataMataData.generated.h"

/**
 * 
 */
// UCLASS()
// class OJOGO_API UMataMataData : public UObject
// {
// 	GENERATED_BODY()
	
// };

USTRUCT(BlueprintType)
struct FFase : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FResultadoData> confrontos;
	
	FFase(){}

	FFase(int32 fase);
};

USTRUCT(BlueprintType)
struct FMataMataData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FFase> fases; // 0 -> oitavas, 1 -> quartas, 2 -> semi, 3 -> final
	
	FMataMataData(){}

	void chaveia(int32 fase);
};