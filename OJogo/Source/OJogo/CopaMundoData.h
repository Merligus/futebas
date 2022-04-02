// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CampeonatoData.h"
#include "TeamData.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 grupo_atualiza_tabela;	// campeonato state

	FORCEINLINE void SetGrupo(int32 grupo) { grupo_atualiza_tabela = grupo; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ind_jogo_atualiza_tabela;	// campeonato state

	FORCEINLINE void SetJogo(int32 jogo) { ind_jogo_atualiza_tabela = jogo; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData team1;	// game mode interface e campeonato state

	FCopaMundoData();

	void sortear(TArray<int32> ranking);

	void bindIndexTimeGrupos();

	void chaveia();

	int32 getGolsTime(int32 index_time) const;

	bool isCampeao(int32 index_time_p) const;
};