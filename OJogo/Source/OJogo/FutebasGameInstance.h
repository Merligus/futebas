// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamData.h"
#include "CopaMundoData.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "FutebasGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UFutebasGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()	

	UPROPERTY()
    TArray<FTeamData> teamsArray;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData team1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData team2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool desempate_por_penaltis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool team1_em_casa;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 grupo_atualiza_tabela;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ind_jogo_atualiza_tabela;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCopaMundoData copa_do_mundo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* teams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float volume;

	void loadTeams();

	FTeamData getTeam(int32 index);

	FTeamData getTeamTrueIndex(int32 index);

	FResultadoData simulaJogo(int32 index_t1, int32 index_t2, bool penaltis);

	void jogaPartida();

	void terminaPartida(FResultadoData r);

	bool simulaJogosProximaRodada();
};
