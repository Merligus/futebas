// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamData.h"
#include "CopaMundoData.h"
#include "LigaData.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "FutebasGameInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class GameMode : uint8 {
	NovaPartida = 0 UMETA(DisplayName = "Nova Partida"),
	CopaMundo = 1 UMETA(DisplayName = "Copa do Mundo"),
	LigaNacoes = 2 UMETA(DisplayName = "Liga Mundial")
};

UCLASS()
class OJOGO_API UFutebasGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()	

	UPROPERTY()
    TArray<FTeamData> teamsArray;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData team1;	// game mode interface e campeonato state
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData team2;	// game mode interface
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool desempate_por_penaltis = true;	// game mode interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool team1_em_casa = true;	// game mode interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool escolheTeam2;	// team select

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 team1_index_slot;	// team select

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 team2_index_slot;	// team select

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	GameMode current_game_mode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCopaMundoData copa_do_mundo;	// campeonato state

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLigaData liga_das_nacoes;	// campeonato state

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* teams;	// seleções

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float volume;

	void loadTeams();

	FTeamData getTeam(int32 index, GameMode game_mode = GameMode::CopaMundo);

	FTeamData getTeamTrueIndex(int32 index);

	FResultadoData simulaJogo(int32 index_t1, int32 index_t2, bool penaltis, GameMode game_mode = GameMode::CopaMundo);

	void jogaPartida(GameMode game_mode);

	void terminaPartida(FResultadoData r, GameMode game_mode);

	bool simulaJogosProximaRodada(GameMode game_mode);
};
