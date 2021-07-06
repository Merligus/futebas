// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CampeonatoData.h"
#include "MataMataData.h"
#include "UObject/NoExportTypes.h"
#include "CopaMundoData.generated.h"

/**
 * 
 */
// UCLASS()
// class OJOGO_API UCopaMundoData : public UObject
// {
// 	GENERATED_BODY()
	
// };

USTRUCT(BlueprintType)
struct FCopaMundoData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
    TMap<int32, int32> sorteioGrupo;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCampeonatoData> tabelaGrupos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMataMataData faseFinal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 fase_atual; // 0 -> grupos, 1 -> oitavas, 2 -> quartas, 3 -> semi, 4 -> final

	FCopaMundoData();

	void sortear();

	void bindIndexTimeGrupos();

	void chaveia();
};