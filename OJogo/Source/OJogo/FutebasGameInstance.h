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
	LigaNacoes = 2 UMETA(DisplayName = "Liga Mundial"),
	NovaPartidaOnline = 3 UMETA(DisplayName = "Nova Partida Online")
};

UENUM(BlueprintType)
enum class TeamsSet : uint8 {
	Times = 0 UMETA(DisplayName = "Times"),
	Selecoes = 1 UMETA(DisplayName = "Seleções")
};

UCLASS()
class OJOGO_API UFutebasGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()	

	UPROPERTY()
    TArray<FTeamData> teamsArray;

	UPROPERTY()
	TArray<FTeamData> teamsRankingArray;

	UPROPERTY()
    TArray<FTeamData> nationalTeamsArray;

	UPROPERTY()
	TArray<FTeamData> nationalTeamsRankingArray;

	void loadClubTeams();

	void loadNationalTeams();

public:

	virtual void PostInitProperties() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData team1;	// game mode interface e campeonato state
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData team2;	// game mode interface
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool desempate_por_penaltis = true;	// game mode interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool team1_em_casa = true;	// game mode interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool vs_bot;	// game mode interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool online_instance;	// game mode interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool escolheTeam2;	// team select

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 team1_index_slot;	// team select

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 team2_index_slot;	// team select

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	GameMode current_game_mode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TeamsSet current_teams_set;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCopaMundoData> copa_do_mundo;	// campeonato state

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLigaData> liga_das_nacoes;	// campeonato state

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* teams;	// times

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* national_teams;	// seleções

	TArray<int32> getTeamsRanking(TeamsSet teams_set);

	void loadTeams();

	FTeamData getTeam(int32 index, GameMode game_mode, TeamsSet teams_set);

	FTeamData getTeamTrueIndex(int32 index, TeamsSet teams_set);

	FResultadoData simulaJogo(int32 index_t1, int32 index_t2, bool penaltis, GameMode game_mode, TeamsSet teams_set);

	UFUNCTION(BlueprintCallable)
	void jogaPartida(GameMode game_mode, TeamsSet teams_set, bool vs_bot_p = true);

	void terminaPartida(FResultadoData r, GameMode game_mode, TeamsSet teams_set);

	bool simulaJogosProximaRodada(GameMode game_mode, TeamsSet teams_set);

	FCopaMundoData* GetCopa(int32 index);

	FLigaData* GetLiga(int32 index);

	void SetCopa(FCopaMundoData copa, int32 index);

	void SetLiga(FLigaData liga, int32 index);
};
