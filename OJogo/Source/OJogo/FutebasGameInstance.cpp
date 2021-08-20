// Fill out your copyright notice in the Description page of Project Settings.


#include "FutebasGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ResultadoData.h"
#include "Math/UnrealMathUtility.h"

void UFutebasGameInstance::PostInitProperties()
{
	Super::PostInitProperties();

	if (teamsArray.Num() == 0)
	{
		FString Context;
		TArray<FTeamData*> aux;

		if (IsValid(teams))
		{
			teams->GetAllRows(Context, aux);
			for (int32 i = 0; i < aux.Num(); ++i)
				teamsArray.Add(*(aux[i]));

			teamsArray.Sort(
				[this](const FTeamData& A, const FTeamData& B) // return true = A antes de B
				{
					if (A.nome_interno == "gambaosaka_jap_png")
						return true;
					else if (B.nome_interno == "gambaosaka_jap_png")
						return false;
					else
						return A.habilidades.overall() > B.habilidades.overall();
				}
			);
			for (int32 i = 0; i < teamsArray.Num(); ++i)
				teamsArray[i].index_time = i;
		}
	}
	if (nationalTeamsArray.Num() == 0)
	{
		FString Context;
		TArray<FTeamData*> aux;

		if (IsValid(national_teams))
		{
			national_teams->GetAllRows(Context, aux);
			for (int32 i = 0; i < aux.Num(); ++i)
				nationalTeamsArray.Add(*(aux[i]));

			nationalTeamsArray.Sort(
				[this](const FTeamData& A, const FTeamData& B) // return true = A antes de B
				{
					if (A.nome_interno == "qatar")
						return true;
					else if (B.nome_interno == "qatar")
						return false;
					else
						return A.habilidades.overall() > B.habilidades.overall();
				}
			);
			for (int32 i = 0; i < nationalTeamsArray.Num(); ++i)
				nationalTeamsArray[i].index_time = i;
		}
	}

	if (nationalTeamsArray.Num() > 0)
	{
		current_teams_set = TeamsSet::Selecoes;
		team1 = nationalTeamsArray[0];
	}
	else if (teamsArray.Num() > 0)
	{
		current_teams_set = TeamsSet::Times;
		team1 = teamsArray[0];
	}
	team2 = team1;
	desempate_por_penaltis = true;
	team1_em_casa = true;
	vs_bot = false;
	online_instance = false;
	escolheTeam2 = false;
	team1_index_slot = -1;
	team2_index_slot = -1;
	current_game_mode = GameMode::NovaPartida;
	copa_do_mundo.Init(FCopaMundoData(), 2);
	liga_das_nacoes.Init(FLigaData(16, 32), 2);
}

void UFutebasGameInstance::loadTeams()
{
	if (teamsArray.Num() == 0)
	{
		FString Context;
		TArray<FTeamData*> aux;

		if (IsValid(teams))
			teams->GetAllRows(Context, aux);
		for (int32 i = 0; i < aux.Num(); ++i)
			teamsArray.Add(*(aux[i]));

		teamsArray.Sort(
			[this](const FTeamData& A, const FTeamData& B) // return true = A antes de B
			{
				if (A.nome_interno == "gambaosaka_jap_png")
					return true;
				else if (B.nome_interno == "gambaosaka_jap_png")
					return false;
				else
					return A.habilidades.overall() > B.habilidades.overall();
			}
		);
		for (int32 i = 0; i < teamsArray.Num(); ++i)
			teamsArray[i].index_time = i;
	}
	if (nationalTeamsArray.Num() == 0)
	{
		FString Context;
		TArray<FTeamData*> aux;

		if (IsValid(national_teams))
			national_teams->GetAllRows(Context, aux);
		for (int32 i = 0; i < aux.Num(); ++i)
			nationalTeamsArray.Add(*(aux[i]));

		nationalTeamsArray.Sort(
			[this](const FTeamData& A, const FTeamData& B) // return true = A antes de B
			{
				if (A.nome_interno == "qatar")
					return true;
				else if (B.nome_interno == "qatar")
					return false;
				else
					return A.habilidades.overall() > B.habilidades.overall();
			}
		);
		for (int32 i = 0; i < nationalTeamsArray.Num(); ++i)
			nationalTeamsArray[i].index_time = i;
	}
}

FTeamData UFutebasGameInstance::getTeam(int32 index, GameMode game_mode, TeamsSet teams_set)
{
	int32 teams_ind((int32)teams_set);
	if (teams_set == TeamsSet::Times)
	{
		if (index < teamsArray.Num())
		{
			if (game_mode == GameMode::CopaMundo)
				return teamsArray[GetCopa(teams_ind)->sorteioGrupo[index]];
			else if (game_mode == GameMode::LigaNacoes)
				return teamsArray[GetLiga(teams_ind)->teamMap[index]];
			else
				return teamsArray[0];
		}
		else
			return FTeamData();
	}
	else if (teams_set == TeamsSet::Selecoes)
	{
		if (index < nationalTeamsArray.Num())
		{
			if (game_mode == GameMode::CopaMundo)
				return nationalTeamsArray[GetCopa(teams_ind)->sorteioGrupo[index]];
			else if (game_mode == GameMode::LigaNacoes)
				return nationalTeamsArray[GetLiga(teams_ind)->teamMap[index]];
			else
				return nationalTeamsArray[0];
		}
		else
			return FTeamData();
	}
	else
		return FTeamData();
}

FTeamData UFutebasGameInstance::getTeamTrueIndex(int32 index, TeamsSet teams_set)
{
	if (teams_set == TeamsSet::Times)
	{
		if (index < teamsArray.Num())
			return teamsArray[index];
		else
			return FTeamData();
	}
	else if (teams_set == TeamsSet::Selecoes)
	{
		if (index < nationalTeamsArray.Num())
			return nationalTeamsArray[index];
		else
			return FTeamData();
	}
	else
		return FTeamData();
}

FResultadoData UFutebasGameInstance::simulaJogo(int32 index_t1, int32 index_t2, bool penaltis, GameMode game_mode, TeamsSet teams_set)
{
	FResultadoData result;
	result.index_casa = index_t1;
	result.index_fora = index_t2;
	float overall_t1 = getTeam(index_t1, game_mode, teams_set).habilidades.overall();
	float overall_t2 = getTeam(index_t2, game_mode, teams_set).habilidades.overall();
	int32 max_gols_t1 = FMath::TruncToInt(FMath::RoundHalfFromZero(10*overall_t1));
	int32 max_gols_t2 = FMath::TruncToInt(FMath::RoundHalfFromZero(10*overall_t2));
	result.gols_casa = FMath::RandRange(FMath::Clamp(max_gols_t1-2, 0, 100), max_gols_t1+2);
	result.gols_fora = FMath::RandRange(FMath::Clamp(max_gols_t2-2, 0, 100), max_gols_t2+2);

	if (result.gols_casa == result.gols_fora && penaltis) // penaltis
	{
		bool vez_casa = FMath::RandBool();
		int32 penaltis_batidos_casa = 0;
		int32 penaltis_batidos_fora = 0;
		result.gols_penalti_casa = 0;
		result.gols_penalti_fora = 0;

		while(true)
		{
			if (vez_casa)
			{
				penaltis_batidos_casa++;
				result.gols_penalti_casa += FMath::RandBool()? 1 : 0;
			}
			else
			{
				penaltis_batidos_fora++;
				result.gols_penalti_fora += FMath::RandBool()? 1 : 0;
			}
			vez_casa = !vez_casa;

			if (penaltis_batidos_casa >= 5 && penaltis_batidos_fora >= 5)
			{
				if (penaltis_batidos_casa == penaltis_batidos_fora)
				{
					if (result.gols_penalti_casa != result.gols_penalti_fora)
						break;
				}
			}
			else
			{
				if ( (result.gols_penalti_casa - result.gols_penalti_fora) > 5-penaltis_batidos_fora)
					break;
				if ( (result.gols_penalti_fora - result.gols_penalti_casa) > 5-penaltis_batidos_casa)
					break;
			}
		}
	}
	return result;
}

void UFutebasGameInstance::jogaPartida(GameMode game_mode, TeamsSet teams_set, bool vs_bot_p)
{
	vs_bot = vs_bot_p;
	current_game_mode = game_mode;
	current_teams_set = teams_set;
	if (team1_em_casa)
		UGameplayStatics::OpenLevel(GetWorld(), FName(team1.estadio));
	else
		UGameplayStatics::OpenLevel(GetWorld(), FName(team2.estadio));

	// debug
	// FResultadoData resultado;
	// resultado.index_casa = team1.index_time;
	// resultado.index_fora = team2.index_time;
	// if (team1_em_casa)
	// {
	//     resultado.gols_casa = 2;
	//     resultado.gols_fora = 1;
	// }
	// else
	// {
	//     resultado.gols_casa = 1;
	//     resultado.gols_fora = 2;
	// }
	// terminaPartida(resultado, current_game_mode, current_teams_set);
}

void UFutebasGameInstance::terminaPartida(FResultadoData r, GameMode game_mode, TeamsSet teams_set)
{
	int32 teams_ind((int32)teams_set);
	if (game_mode == GameMode::CopaMundo)
	{
		if (GetCopa(teams_ind)->fase_atual == 0)
		{
			int32 rodada_atual = GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].rodada_atual;
			r.index_casa = GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].calendario[rodada_atual].jogos[GetCopa(teams_ind)->ind_jogo_atualiza_tabela].casa;
			r.index_fora = GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].calendario[rodada_atual].jogos[GetCopa(teams_ind)->ind_jogo_atualiza_tabela].fora;
			if (rodada_atual == 2)
			{
				int32 index_t1_temp = GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].calendario[2].jogos[(GetCopa(teams_ind)->ind_jogo_atualiza_tabela+1)%2].casa;
				int32 index_t2_temp = GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].calendario[2].jogos[(GetCopa(teams_ind)->ind_jogo_atualiza_tabela+1)%2].fora;
				FResultadoData r_temp = simulaJogo(index_t1_temp, index_t2_temp, false, game_mode, teams_set);
				GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].atualizaTabela(r_temp.index_casa, r_temp.index_fora, r_temp.gols_casa, r_temp.gols_fora);
				GetCopa(teams_ind)->SetJogo(1);
			}
				
			GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].atualizaTabela(r.index_casa, r.index_fora, r.gols_casa, r.gols_fora);
			GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].terminaRodada();
			GetCopa(teams_ind)->SetJogo(GetCopa(teams_ind)->ind_jogo_atualiza_tabela + 1);
			if (GetCopa(teams_ind)->ind_jogo_atualiza_tabela == 2)
			{
				if (GetCopa(teams_ind)->grupo_atualiza_tabela == 7 && GetCopa(teams_ind)->tabelaGrupos[7].rodada_atual == 3)
					GetCopa(teams_ind)->chaveia();
				GetCopa(teams_ind)->SetJogo(0);
				GetCopa(teams_ind)->SetGrupo(GetCopa(teams_ind)->grupo_atualiza_tabela + 1);
			}
			GetCopa(teams_ind)->SetGrupo(GetCopa(teams_ind)->grupo_atualiza_tabela % GetCopa(teams_ind)->tabelaGrupos.Num());
		}
		else if (GetCopa(teams_ind)->fase_atual <= 4)
		{
			r.index_casa = GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-1].confrontos[GetCopa(teams_ind)->ind_jogo_atualiza_tabela].index_casa;
			r.index_fora = GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-1].confrontos[GetCopa(teams_ind)->ind_jogo_atualiza_tabela].index_fora;
			GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-1].confrontos[GetCopa(teams_ind)->ind_jogo_atualiza_tabela] = r;
			GetCopa(teams_ind)->SetJogo(GetCopa(teams_ind)->ind_jogo_atualiza_tabela + 1);
			if (GetCopa(teams_ind)->ind_jogo_atualiza_tabela == GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-1].confrontos.Num())
			{
				GetCopa(teams_ind)->chaveia();
				GetCopa(teams_ind)->SetJogo(0);
			}
		}
	}
	else if (game_mode == GameMode::LigaNacoes)
	{
		int32 rodada_atual = GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].rodada_atual;
		int32 rodada_final = GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].calendario.Num() - 1;
		int32 jogos = GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].calendario[rodada_atual].jogos.Num();
		r.index_casa = GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].calendario[rodada_atual].jogos[GetLiga(teams_ind)->ind_jogo_atualiza_tabela].casa;
		r.index_fora = GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].calendario[rodada_atual].jogos[GetLiga(teams_ind)->ind_jogo_atualiza_tabela].fora;
		if (rodada_atual == rodada_final)
		{
			for (int32 grupo_atualiza_tabela_temp = 0; grupo_atualiza_tabela_temp < GetLiga(teams_ind)->tabelas.Num(); ++grupo_atualiza_tabela_temp)
			{
				int32 rodada_atual_temp = GetLiga(teams_ind)->tabelas[grupo_atualiza_tabela_temp].rodada_atual;
				for (int32 ind_jogo_atualiza_tabela_temp = 0; ind_jogo_atualiza_tabela_temp < GetLiga(teams_ind)->tabelas[grupo_atualiza_tabela_temp].calendario[rodada_atual_temp].jogos.Num(); ++ind_jogo_atualiza_tabela_temp)
				{
					if (ind_jogo_atualiza_tabela_temp != GetLiga(teams_ind)->ind_jogo_atualiza_tabela || grupo_atualiza_tabela_temp != GetLiga(teams_ind)->grupo_atualiza_tabela)
					{
						int32 index_t1_temp = GetLiga(teams_ind)->tabelas[grupo_atualiza_tabela_temp].calendario[rodada_final].jogos[ind_jogo_atualiza_tabela_temp].casa;
						int32 index_t2_temp = GetLiga(teams_ind)->tabelas[grupo_atualiza_tabela_temp].calendario[rodada_final].jogos[ind_jogo_atualiza_tabela_temp].fora;
						FResultadoData r_temp = simulaJogo(index_t1_temp, index_t2_temp, false, game_mode, teams_set);
						GetLiga(teams_ind)->tabelas[grupo_atualiza_tabela_temp].atualizaTabela(r_temp.index_casa, r_temp.index_fora, r_temp.gols_casa, r_temp.gols_fora);
					}
				}
				GetLiga(teams_ind)->tabelas[grupo_atualiza_tabela_temp].terminaRodada();
			}
		}
			
		GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].atualizaTabela(r.index_casa, r.index_fora, r.gols_casa, r.gols_fora);
		GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].terminaRodada();
		GetLiga(teams_ind)->SetJogo(GetLiga(teams_ind)->ind_jogo_atualiza_tabela + 1);
		if (GetLiga(teams_ind)->ind_jogo_atualiza_tabela == jogos)
		{
			GetLiga(teams_ind)->SetJogo(0);
			GetLiga(teams_ind)->SetGrupo(GetLiga(teams_ind)->grupo_atualiza_tabela + 1);
		}
		GetLiga(teams_ind)->SetGrupo(GetLiga(teams_ind)->grupo_atualiza_tabela % GetLiga(teams_ind)->tabelas.Num());
	}
}

bool UFutebasGameInstance::simulaJogosProximaRodada(GameMode game_mode, TeamsSet teams_set)
{
	int32 teams_ind((int32)teams_set);
	if (game_mode == GameMode::CopaMundo)
	{
		if (GetCopa(teams_ind)->fase_atual == 0)
		{
			for (; GetCopa(teams_ind)->grupo_atualiza_tabela < GetCopa(teams_ind)->tabelaGrupos.Num(); GetCopa(teams_ind)->SetGrupo(GetCopa(teams_ind)->grupo_atualiza_tabela + 1))
			{
				int32 rodada_atual = GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].rodada_atual;
				for (; GetCopa(teams_ind)->ind_jogo_atualiza_tabela < GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].calendario[rodada_atual].jogos.Num(); GetCopa(teams_ind)->SetJogo(GetCopa(teams_ind)->ind_jogo_atualiza_tabela + 1))
				{
					int32 index_t1 = GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].calendario[rodada_atual].jogos[GetCopa(teams_ind)->ind_jogo_atualiza_tabela].casa;
					int32 index_t2 = GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].calendario[rodada_atual].jogos[GetCopa(teams_ind)->ind_jogo_atualiza_tabela].fora;
					FResultadoData r;
					if (GetCopa(teams_ind)->sorteioGrupo[index_t1] == team1.index_time)
					{
						desempate_por_penaltis = false;
						team2 = getTeam(index_t2, game_mode, teams_set);
						team2.index_time = GetCopa(teams_ind)->sorteioGrupo[index_t2];

						team1_em_casa = true;
						
						if (GetCopa(teams_ind)->ind_jogo_atualiza_tabela >= 1)
							GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].terminaRodada();
						return true;
					}
					else if (GetCopa(teams_ind)->sorteioGrupo[index_t2] == team1.index_time)
					{
						desempate_por_penaltis = false;
						team2 = getTeam(index_t1, game_mode, teams_set);
						team2.index_time = GetCopa(teams_ind)->sorteioGrupo[index_t1];

						team1_em_casa = false;
						
						if (GetCopa(teams_ind)->ind_jogo_atualiza_tabela >= 1)
							GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].terminaRodada();
						return true;
					}
					else
					{
						if (GetCopa(teams_ind)->ind_jogo_atualiza_tabela == 0 && rodada_atual == 2)
						{
							int32 index_t1_temp = GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].calendario[2].jogos[1].casa;
							int32 index_t2_temp = GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].calendario[2].jogos[1].fora;
							if (GetCopa(teams_ind)->sorteioGrupo[index_t1_temp] == team1.index_time)
							{
								desempate_por_penaltis = false;
								team2 = getTeam(index_t2_temp, game_mode, teams_set);
								team2.index_time = GetCopa(teams_ind)->sorteioGrupo[index_t2_temp];

								team1_em_casa = true;
								GetCopa(teams_ind)->SetJogo(1);
								return true;
							}
							else if (GetCopa(teams_ind)->sorteioGrupo[index_t2_temp] == team1.index_time)
							{
								desempate_por_penaltis = false;
								team2 = getTeam(index_t1_temp, game_mode, teams_set);
								team2.index_time = GetCopa(teams_ind)->sorteioGrupo[index_t1_temp];

								team1_em_casa = false;
								GetCopa(teams_ind)->SetJogo(1);
								return true;
							}
						}
						r = simulaJogo(index_t1, index_t2, false, game_mode, teams_set);
					}
					GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].atualizaTabela(index_t1, index_t2, r.gols_casa, r.gols_fora);
				}
				GetCopa(teams_ind)->SetJogo(0);
				GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].terminaRodada();
			}
			if (GetCopa(teams_ind)->tabelaGrupos[7].rodada_atual == 3)
				GetCopa(teams_ind)->chaveia();
			GetCopa(teams_ind)->SetGrupo(0);
			
			// debug
			// for (int32 grupo = 0; grupo < GetCopa(teams_ind)->tabelaGrupos.Num(); ++grupo)
			// {
			//     FString JoinedStrRodada("Grupo ");
			//     JoinedStrRodada += FString::FromInt(grupo);
			//     JoinedStrRodada += FString(" Rodada ");
			//     int32 rodada = GetCopa(teams_ind)->tabelaGrupos[grupo].rodada_atual - 1;
			//     JoinedStrRodada += FString::FromInt(rodada);
			//     for (int32 indJogo = 0; indJogo < GetCopa(teams_ind)->tabelaGrupos[grupo].calendario[rodada].jogos.Num(); ++indJogo)
			//     {
			//         int32 index_casa = GetCopa(teams_ind)->tabelaGrupos[grupo].calendario[rodada].jogos[indJogo].casa;
			//         int32 index_fora = GetCopa(teams_ind)->tabelaGrupos[grupo].calendario[rodada].jogos[indJogo].fora;
			//         int32 gols_casa = GetCopa(teams_ind)->tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_casa;
			//         int32 gols_fora = GetCopa(teams_ind)->tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_fora;
			//         JoinedStrRodada += TEXT(" (");
			//         JoinedStrRodada += getTeam(index_casa, game_mode, teams_set).nome_hud;
			//         JoinedStrRodada += TEXT(" ");
			//         JoinedStrRodada += FString::FromInt(gols_casa);
			//         JoinedStrRodada += TEXT(" x ");
			//         JoinedStrRodada += FString::FromInt(gols_fora);
			//         JoinedStrRodada += TEXT(" ");
			//         JoinedStrRodada += getTeam(index_fora, game_mode, teams_set).nome_hud;
			//         JoinedStrRodada += TEXT(")");
			//     }
			//     GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, JoinedStrRodada);
			// }
		}
		else if (GetCopa(teams_ind)->fase_atual <= 4)
		{
			GetCopa(teams_ind)->SetJogo(GetCopa(teams_ind)->ind_jogo_atualiza_tabela % GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-1].confrontos.Num());
			for (; GetCopa(teams_ind)->ind_jogo_atualiza_tabela < GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-1].confrontos.Num(); GetCopa(teams_ind)->SetJogo(GetCopa(teams_ind)->ind_jogo_atualiza_tabela + 1))
			{
				int32 index_t1 = GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-1].confrontos[GetCopa(teams_ind)->ind_jogo_atualiza_tabela].index_casa;
				int32 index_t2 = GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-1].confrontos[GetCopa(teams_ind)->ind_jogo_atualiza_tabela].index_fora;
				if (GetCopa(teams_ind)->sorteioGrupo[index_t1] == team1.index_time)
				{
					desempate_por_penaltis = true;
					team2 = getTeam(index_t2, game_mode, teams_set);
					team2.index_time = GetCopa(teams_ind)->sorteioGrupo[index_t2];

					team1_em_casa = true;
					
					// GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].terminaRodada();
					return true;
				}
				else if (GetCopa(teams_ind)->sorteioGrupo[index_t2] == team1.index_time)
				{
					desempate_por_penaltis = true;
					team2 = getTeam(index_t1, game_mode, teams_set);
					team2.index_time = GetCopa(teams_ind)->sorteioGrupo[index_t1];

					team1_em_casa = false;
					
					// GetCopa(teams_ind)->tabelaGrupos[GetCopa(teams_ind)->grupo_atualiza_tabela].terminaRodada();
					return true;
				}
				else
					GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-1].confrontos[GetCopa(teams_ind)->ind_jogo_atualiza_tabela] = simulaJogo(index_t1, index_t2, true, game_mode, teams_set);
			}

			GetCopa(teams_ind)->chaveia();
			GetCopa(teams_ind)->SetJogo(0);

			// debug
			// for (int32 ind_jogo = 0; ind_jogo < GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-2].confrontos.Num(); ++ind_jogo)
			// {
			//     FString JoinedStrRodada("Jogo ");
			//     JoinedStrRodada += FString::FromInt(ind_jogo);
			//     int32 index_casa = GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-2].confrontos[ind_jogo].index_casa;
			//     int32 index_fora = GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-2].confrontos[ind_jogo].index_fora;
			//     int32 gols_casa = GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-2].confrontos[ind_jogo].gols_casa;
			//     int32 gols_fora = GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-2].confrontos[ind_jogo].gols_fora;
			//     int32 gols_penalti_casa = GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-2].confrontos[ind_jogo].gols_penalti_casa;
			//     int32 gols_penalti_fora = GetCopa(teams_ind)->faseFinal.fases[GetCopa(teams_ind)->fase_atual-2].confrontos[ind_jogo].gols_penalti_fora;
			//     JoinedStrRodada += TEXT(" [");
			//     JoinedStrRodada += getTeam(index_casa, game_mode, teams_set).nome_hud;
			//     JoinedStrRodada += TEXT(" ");
			//     JoinedStrRodada += FString::FromInt(gols_casa);
			//     if (gols_penalti_casa != -1)
			//     {
			//         JoinedStrRodada += TEXT(" (");
			//         JoinedStrRodada += FString::FromInt(gols_penalti_casa);
			//         JoinedStrRodada += TEXT(")");
			//     }
			//     JoinedStrRodada += TEXT(" x ");
			//     if (gols_penalti_fora != -1)
			//     {
			//         JoinedStrRodada += TEXT("(");
			//         JoinedStrRodada += FString::FromInt(gols_penalti_fora);
			//         JoinedStrRodada += TEXT(") ");
			//     }
			//     JoinedStrRodada += FString::FromInt(gols_fora);
			//     JoinedStrRodada += TEXT(" ");
			//     JoinedStrRodada += getTeam(index_fora, game_mode, teams_set).nome_hud;
			//     JoinedStrRodada += TEXT("]");
			//     GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, JoinedStrRodada);
			// }
		}
	}
	else if (game_mode == GameMode::LigaNacoes)
	{
		bool termina(true);
		for (int32 divisao = 0; divisao < GetLiga(teams_ind)->divisoes; ++divisao)
			termina = termina && (GetLiga(teams_ind)->tabelas[divisao].rodada_atual == GetLiga(teams_ind)->tabelas[divisao].calendario.Num());
		if (!termina)
		{
			for (; GetLiga(teams_ind)->grupo_atualiza_tabela < GetLiga(teams_ind)->tabelas.Num(); GetLiga(teams_ind)->SetGrupo(GetLiga(teams_ind)->grupo_atualiza_tabela + 1))
			{
				int32 rodada_atual = GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].rodada_atual;
				int32 jogos = GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].calendario[rodada_atual].jogos.Num();
				for (; GetLiga(teams_ind)->ind_jogo_atualiza_tabela < jogos; GetLiga(teams_ind)->SetJogo(GetLiga(teams_ind)->ind_jogo_atualiza_tabela + 1))
				{
					int32 index_t1 = GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].calendario[rodada_atual].jogos[GetLiga(teams_ind)->ind_jogo_atualiza_tabela].casa;
					int32 index_t2 = GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].calendario[rodada_atual].jogos[GetLiga(teams_ind)->ind_jogo_atualiza_tabela].fora;
					FResultadoData r;
					if (GetLiga(teams_ind)->teamMap[index_t1] == team1.index_time)
					{
						desempate_por_penaltis = false;
						team2 = getTeam(index_t2, game_mode, teams_set);
						team2.index_time = GetLiga(teams_ind)->teamMap[index_t2];

						team1_em_casa = true;
						
						if (GetLiga(teams_ind)->ind_jogo_atualiza_tabela >= 1)
							GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].terminaRodada();
						return true;
					}
					else if (GetLiga(teams_ind)->teamMap[index_t2] == team1.index_time)
					{
						desempate_por_penaltis = false;
						team2 = getTeam(index_t1, game_mode, teams_set);
						team2.index_time = GetLiga(teams_ind)->teamMap[index_t1];

						team1_em_casa = false;
						
						if (GetLiga(teams_ind)->ind_jogo_atualiza_tabela >= 1)
							GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].terminaRodada();
						return true;
					}
					else
					{
						int32 rodada_final = GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].calendario.Num() - 1;
						if (rodada_atual != rodada_final)
						{
							r = simulaJogo(index_t1, index_t2, false, game_mode, teams_set);
							GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].atualizaTabela(index_t1, index_t2, r.gols_casa, r.gols_fora);
						}
					}
				}
				GetLiga(teams_ind)->SetJogo(0);
				GetLiga(teams_ind)->tabelas[GetLiga(teams_ind)->grupo_atualiza_tabela].terminaRodada();
			}
			GetLiga(teams_ind)->SetGrupo(0);
			
			// debug
			// for (int32 grupo = 0; grupo < GetLiga(teams_ind)->tabelas.Num(); ++grupo)
			// {
			//     FString JoinedStrRodada("Divisao ");
			//     JoinedStrRodada += FString::FromInt(grupo);
			//     JoinedStrRodada += FString(" Rodada ");
			//     int32 rodada = GetLiga(teams_ind)->tabelas[grupo].rodada_atual - 1;
			//     JoinedStrRodada += FString::FromInt(rodada);
			//     for (int32 indJogo = 0; indJogo < GetLiga(teams_ind)->tabelas[grupo].calendario[rodada].jogos.Num(); ++indJogo)
			//     {
			//         int32 index_casa = GetLiga(teams_ind)->tabelas[grupo].calendario[rodada].jogos[indJogo].casa;
			//         int32 index_fora = GetLiga(teams_ind)->tabelas[grupo].calendario[rodada].jogos[indJogo].fora;
			//         int32 gols_casa = GetLiga(teams_ind)->tabelas[grupo].confrontos.casa[index_casa].fora[index_fora].gols_casa;
			//         int32 gols_fora = GetLiga(teams_ind)->tabelas[grupo].confrontos.casa[index_casa].fora[index_fora].gols_fora;
			//         JoinedStrRodada += TEXT(" (");
			//         JoinedStrRodada += getTeam(index_casa, game_mode).nome_hud;
			//         JoinedStrRodada += TEXT(" ");
			//         JoinedStrRodada += FString::FromInt(gols_casa);
			//         JoinedStrRodada += TEXT(" x ");
			//         JoinedStrRodada += FString::FromInt(gols_fora);
			//         JoinedStrRodada += TEXT(" ");
			//         JoinedStrRodada += getTeam(index_fora, game_mode).nome_hud;
			//         JoinedStrRodada += TEXT(")");
			//     }
			//     GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, JoinedStrRodada);
			// }
		}
		else
		{
			GetLiga(teams_ind)->terminaTemporada();
			GetLiga(teams_ind)->SetJogo(0);
			GetLiga(teams_ind)->SetGrupo(0);
		}
	}
	return false;
}

FCopaMundoData* UFutebasGameInstance::GetCopa(int32 index)
{
	while(index >= copa_do_mundo.Num())
		copa_do_mundo.Add(FCopaMundoData());
	return &copa_do_mundo[index];
}

FLigaData* UFutebasGameInstance::GetLiga(int32 index)
{
	while(index >= liga_das_nacoes.Num())
		liga_das_nacoes.Add(FLigaData());
	return &liga_das_nacoes[index];
}

void UFutebasGameInstance::SetCopa(FCopaMundoData copa, int32 index)
{
	while(index >= copa_do_mundo.Num())
		copa_do_mundo.Add(FCopaMundoData());
	copa_do_mundo[index] = copa;
}

void UFutebasGameInstance::SetLiga(FLigaData liga, int32 index)
{
	while(index >= liga_das_nacoes.Num())
		liga_das_nacoes.Add(FLigaData());
	liga_das_nacoes[index] = liga;
}