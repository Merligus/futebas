// Fill out your copyright notice in the Description page of Project Settings.


#include "FutebasGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ResultadoData.h"
#include "Math/UnrealMathUtility.h"

void UFutebasGameInstance::loadTeams()
{
    if (teamsArray.Num() == 0)
    {
        FString Context;
        TArray<FTeamData*> aux;

        teams->GetAllRows(Context, aux);
        for (int32 i = 0; i < aux.Num(); ++i)
            teamsArray.Add(*(aux[i]));

        teamsArray.Sort(
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
        for (int32 i = 0; i < teamsArray.Num(); ++i)
            teamsArray[i].index_time = i;
    }
}

FTeamData UFutebasGameInstance::getTeam(int32 index, GameMode game_mode)
{
    if (index < teamsArray.Num())
    {
        if (game_mode == GameMode::CopaMundo)
            return teamsArray[copa_do_mundo.sorteioGrupo[index]];
        else if (game_mode == GameMode::LigaNacoes)
            return teamsArray[liga_das_nacoes.teamMap[index]];
        else
            return teamsArray[0];
    }
    else
        return FTeamData();
}

FTeamData UFutebasGameInstance::getTeamTrueIndex(int32 index)
{
    if (index < teamsArray.Num())
        return teamsArray[index];
    else
        return FTeamData();
}

FResultadoData UFutebasGameInstance::simulaJogo(int32 index_t1, int32 index_t2, bool penaltis, GameMode game_mode)
{
    FResultadoData result;
    result.index_casa = index_t1;
    result.index_fora = index_t2;
    float overall_t1 = getTeam(index_t1, game_mode).habilidades.overall();
    float overall_t2 = getTeam(index_t2, game_mode).habilidades.overall();
    int32 max_gols_t1 = FMath::TruncToInt(FMath::RoundHalfFromZero(5*overall_t1));
    int32 max_gols_t2 = FMath::TruncToInt(FMath::RoundHalfFromZero(5*overall_t2));
    result.gols_casa = FMath::RandRange(FMath::Clamp(max_gols_t1-1, 0, 100), max_gols_t1+1);
    result.gols_fora = FMath::RandRange(FMath::Clamp(max_gols_t2-1, 0, 100), max_gols_t2+1);

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

void UFutebasGameInstance::jogaPartida(GameMode game_mode)
{
    current_game_mode = game_mode;
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
    // terminaPartida(resultado, GameMode::LigaNacoes);
}

void UFutebasGameInstance::terminaPartida(FResultadoData r, GameMode game_mode)
{
    if (game_mode == GameMode::CopaMundo)
    {
        if (copa_do_mundo.fase_atual == 0)
        {
            int32 rodada_atual = copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].rodada_atual;
            r.index_casa = copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].calendario[rodada_atual].jogos[copa_do_mundo.ind_jogo_atualiza_tabela].casa;
            r.index_fora = copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].calendario[rodada_atual].jogos[copa_do_mundo.ind_jogo_atualiza_tabela].fora;
            if (rodada_atual == 2)
            {
                int32 index_t1_temp = copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].calendario[2].jogos[(copa_do_mundo.ind_jogo_atualiza_tabela+1)%2].casa;
                int32 index_t2_temp = copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].calendario[2].jogos[(copa_do_mundo.ind_jogo_atualiza_tabela+1)%2].fora;
                FResultadoData r_temp = simulaJogo(index_t1_temp, index_t2_temp, false, game_mode);
                copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].atualizaTabela(r_temp.index_casa, r_temp.index_fora, r_temp.gols_casa, r_temp.gols_fora);
                copa_do_mundo.ind_jogo_atualiza_tabela = 1;
            }
                
            copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].atualizaTabela(r.index_casa, r.index_fora, r.gols_casa, r.gols_fora);
            copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].terminaRodada();
            ++copa_do_mundo.ind_jogo_atualiza_tabela;
            if (copa_do_mundo.ind_jogo_atualiza_tabela == 2)
            {
                if (copa_do_mundo.grupo_atualiza_tabela == 7 && copa_do_mundo.tabelaGrupos[7].rodada_atual == 3)
                    copa_do_mundo.chaveia();
                copa_do_mundo.ind_jogo_atualiza_tabela = 0;
                copa_do_mundo.grupo_atualiza_tabela++;
            }
            copa_do_mundo.grupo_atualiza_tabela = copa_do_mundo.grupo_atualiza_tabela % copa_do_mundo.tabelaGrupos.Num();
        }
        else if (copa_do_mundo.fase_atual <= 4)
        {
            r.index_casa = copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-1].confrontos[copa_do_mundo.ind_jogo_atualiza_tabela].index_casa;
            r.index_fora = copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-1].confrontos[copa_do_mundo.ind_jogo_atualiza_tabela].index_fora;
            copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-1].confrontos[copa_do_mundo.ind_jogo_atualiza_tabela] = r;
            ++copa_do_mundo.ind_jogo_atualiza_tabela;
            if (copa_do_mundo.ind_jogo_atualiza_tabela == copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-1].confrontos.Num())
            {
                copa_do_mundo.chaveia();
                copa_do_mundo.ind_jogo_atualiza_tabela = 0;
            }
        }
    }
    else if (game_mode == GameMode::LigaNacoes)
    {
        int32 rodada_atual = liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].rodada_atual;
        int32 rodada_final = liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].calendario.Num() - 1;
        int32 jogos = liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].calendario[rodada_atual].jogos.Num();
        r.index_casa = liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].calendario[rodada_atual].jogos[liga_das_nacoes.ind_jogo_atualiza_tabela].casa;
        r.index_fora = liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].calendario[rodada_atual].jogos[liga_das_nacoes.ind_jogo_atualiza_tabela].fora;
        if (rodada_atual == rodada_final)
        {
            for (int32 grupo_atualiza_tabela_temp = 0; grupo_atualiza_tabela_temp < liga_das_nacoes.tabelas.Num(); ++grupo_atualiza_tabela_temp)
            {
                int32 rodada_atual_temp = liga_das_nacoes.tabelas[grupo_atualiza_tabela_temp].rodada_atual;
                for (int32 ind_jogo_atualiza_tabela_temp = 0; ind_jogo_atualiza_tabela_temp < liga_das_nacoes.tabelas[grupo_atualiza_tabela_temp].calendario[rodada_atual_temp].jogos.Num(); ++ind_jogo_atualiza_tabela_temp)
                {
                    if (ind_jogo_atualiza_tabela_temp != liga_das_nacoes.ind_jogo_atualiza_tabela || grupo_atualiza_tabela_temp != liga_das_nacoes.grupo_atualiza_tabela)
                    {
                        int32 index_t1_temp = liga_das_nacoes.tabelas[grupo_atualiza_tabela_temp].calendario[rodada_final].jogos[ind_jogo_atualiza_tabela_temp].casa;
                        int32 index_t2_temp = liga_das_nacoes.tabelas[grupo_atualiza_tabela_temp].calendario[rodada_final].jogos[ind_jogo_atualiza_tabela_temp].fora;
                        FResultadoData r_temp = simulaJogo(index_t1_temp, index_t2_temp, false, game_mode);
                        liga_das_nacoes.tabelas[grupo_atualiza_tabela_temp].atualizaTabela(r_temp.index_casa, r_temp.index_fora, r_temp.gols_casa, r_temp.gols_fora);
                    }
                }
                liga_das_nacoes.tabelas[grupo_atualiza_tabela_temp].terminaRodada();
            }
        }
            
        liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].atualizaTabela(r.index_casa, r.index_fora, r.gols_casa, r.gols_fora);
        liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].terminaRodada();
        ++liga_das_nacoes.ind_jogo_atualiza_tabela;
        if (liga_das_nacoes.ind_jogo_atualiza_tabela == jogos)
        {
            liga_das_nacoes.ind_jogo_atualiza_tabela = 0;
            liga_das_nacoes.grupo_atualiza_tabela++;
        }
        liga_das_nacoes.grupo_atualiza_tabela = liga_das_nacoes.grupo_atualiza_tabela % liga_das_nacoes.tabelas.Num();
    }
}

bool UFutebasGameInstance::simulaJogosProximaRodada(GameMode game_mode)
{
    if (game_mode == GameMode::CopaMundo)
    {
        if (copa_do_mundo.fase_atual == 0)
        {
            for (; copa_do_mundo.grupo_atualiza_tabela < copa_do_mundo.tabelaGrupos.Num(); ++copa_do_mundo.grupo_atualiza_tabela)
            {
                int32 rodada_atual = copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].rodada_atual;
                for (; copa_do_mundo.ind_jogo_atualiza_tabela < copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].calendario[rodada_atual].jogos.Num(); ++copa_do_mundo.ind_jogo_atualiza_tabela)
                {
                    int32 index_t1 = copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].calendario[rodada_atual].jogos[copa_do_mundo.ind_jogo_atualiza_tabela].casa;
                    int32 index_t2 = copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].calendario[rodada_atual].jogos[copa_do_mundo.ind_jogo_atualiza_tabela].fora;
                    FResultadoData r;
                    if (copa_do_mundo.sorteioGrupo[index_t1] == team1.index_time)
                    {
                        desempate_por_penaltis = false;
                        team2 = getTeam(index_t2);
                        team2.index_time = copa_do_mundo.sorteioGrupo[index_t2];

                        team1_em_casa = true;
                        
                        if (copa_do_mundo.ind_jogo_atualiza_tabela >= 1)
                            copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].terminaRodada();
                        return true;
                    }
                    else if (copa_do_mundo.sorteioGrupo[index_t2] == team1.index_time)
                    {
                        desempate_por_penaltis = false;
                        team2 = getTeam(index_t1);
                        team2.index_time = copa_do_mundo.sorteioGrupo[index_t1];

                        team1_em_casa = false;
                        
                        if (copa_do_mundo.ind_jogo_atualiza_tabela >= 1)
                            copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].terminaRodada();
                        return true;
                    }
                    else
                    {
                        if (copa_do_mundo.ind_jogo_atualiza_tabela == 0 && rodada_atual == 2)
                        {
                            int32 index_t1_temp = copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].calendario[2].jogos[1].casa;
                            int32 index_t2_temp = copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].calendario[2].jogos[1].fora;
                            if (copa_do_mundo.sorteioGrupo[index_t1_temp] == team1.index_time)
                            {
                                desempate_por_penaltis = false;
                                team2 = getTeam(index_t2_temp);
                                team2.index_time = copa_do_mundo.sorteioGrupo[index_t2_temp];

                                team1_em_casa = true;
                                copa_do_mundo.ind_jogo_atualiza_tabela = 1;
                                return true;
                            }
                            else if (copa_do_mundo.sorteioGrupo[index_t2_temp] == team1.index_time)
                            {
                                desempate_por_penaltis = false;
                                team2 = getTeam(index_t1_temp);
                                team2.index_time = copa_do_mundo.sorteioGrupo[index_t1_temp];

                                team1_em_casa = false;
                                copa_do_mundo.ind_jogo_atualiza_tabela = 1;
                                return true;
                            }
                        }
                        r = simulaJogo(index_t1, index_t2, false, game_mode);
                    }
                    copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].atualizaTabela(index_t1, index_t2, r.gols_casa, r.gols_fora);
                }
                copa_do_mundo.ind_jogo_atualiza_tabela = 0;
                copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].terminaRodada();
            }
            if (copa_do_mundo.tabelaGrupos[7].rodada_atual == 3)
                copa_do_mundo.chaveia();
            copa_do_mundo.grupo_atualiza_tabela = 0;
            
            // debug
            // for (int32 grupo = 0; grupo < copa_do_mundo.tabelaGrupos.Num(); ++grupo)
            // {
            //     FString JoinedStrRodada("Grupo ");
            //     JoinedStrRodada += FString::FromInt(grupo);
            //     JoinedStrRodada += FString(" Rodada ");
            //     int32 rodada = copa_do_mundo.tabelaGrupos[grupo].rodada_atual - 1;
            //     JoinedStrRodada += FString::FromInt(rodada);
            //     for (int32 indJogo = 0; indJogo < copa_do_mundo.tabelaGrupos[grupo].calendario[rodada].jogos.Num(); ++indJogo)
            //     {
            //         int32 index_casa = copa_do_mundo.tabelaGrupos[grupo].calendario[rodada].jogos[indJogo].casa;
            //         int32 index_fora = copa_do_mundo.tabelaGrupos[grupo].calendario[rodada].jogos[indJogo].fora;
            //         int32 gols_casa = copa_do_mundo.tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_casa;
            //         int32 gols_fora = copa_do_mundo.tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_fora;
            //         JoinedStrRodada += TEXT(" (");
            //         JoinedStrRodada += getTeam(index_casa).nome_hud;
            //         JoinedStrRodada += TEXT(" ");
            //         JoinedStrRodada += FString::FromInt(gols_casa);
            //         JoinedStrRodada += TEXT(" x ");
            //         JoinedStrRodada += FString::FromInt(gols_fora);
            //         JoinedStrRodada += TEXT(" ");
            //         JoinedStrRodada += getTeam(index_fora).nome_hud;
            //         JoinedStrRodada += TEXT(")");
            //     }
            //     GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, JoinedStrRodada);
            // }
        }
        else if (copa_do_mundo.fase_atual <= 4)
        {
            copa_do_mundo.ind_jogo_atualiza_tabela = copa_do_mundo.ind_jogo_atualiza_tabela % copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-1].confrontos.Num();
            for (; copa_do_mundo.ind_jogo_atualiza_tabela < copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-1].confrontos.Num(); ++copa_do_mundo.ind_jogo_atualiza_tabela)
            {
                int32 index_t1 = copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-1].confrontos[copa_do_mundo.ind_jogo_atualiza_tabela].index_casa;
                int32 index_t2 = copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-1].confrontos[copa_do_mundo.ind_jogo_atualiza_tabela].index_fora;
                if (copa_do_mundo.sorteioGrupo[index_t1] == team1.index_time)
                {
                    desempate_por_penaltis = true;
                    team2 = getTeam(index_t2);
                    team2.index_time = copa_do_mundo.sorteioGrupo[index_t2];

                    team1_em_casa = true;
                    
                    copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].terminaRodada();
                    return true;
                }
                else if (copa_do_mundo.sorteioGrupo[index_t2] == team1.index_time)
                {
                    desempate_por_penaltis = true;
                    team2 = getTeam(index_t1);
                    team2.index_time = copa_do_mundo.sorteioGrupo[index_t1];

                    team1_em_casa = false;
                    
                    copa_do_mundo.tabelaGrupos[copa_do_mundo.grupo_atualiza_tabela].terminaRodada();
                    return true;
                }
                else
                    copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-1].confrontos[copa_do_mundo.ind_jogo_atualiza_tabela] = simulaJogo(index_t1, index_t2, true, game_mode);
            }

            copa_do_mundo.chaveia();
            copa_do_mundo.ind_jogo_atualiza_tabela = 0;

            // debug
            // for (int32 ind_jogo = 0; ind_jogo < copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-2].confrontos.Num(); ++ind_jogo)
            // {
            //     FString JoinedStrRodada("Jogo ");
            //     JoinedStrRodada += FString::FromInt(ind_jogo);
            //     int32 index_casa = copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-2].confrontos[ind_jogo].index_casa;
            //     int32 index_fora = copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-2].confrontos[ind_jogo].index_fora;
            //     int32 gols_casa = copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-2].confrontos[ind_jogo].gols_casa;
            //     int32 gols_fora = copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-2].confrontos[ind_jogo].gols_fora;
            //     int32 gols_penalti_casa = copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-2].confrontos[ind_jogo].gols_penalti_casa;
            //     int32 gols_penalti_fora = copa_do_mundo.faseFinal.fases[copa_do_mundo.fase_atual-2].confrontos[ind_jogo].gols_penalti_fora;
            //     JoinedStrRodada += TEXT(" [");
            //     JoinedStrRodada += getTeam(index_casa).nome_hud;
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
            //     JoinedStrRodada += getTeam(index_fora).nome_hud;
            //     JoinedStrRodada += TEXT("]");
            //     GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, JoinedStrRodada);
            // }
        }
    }
    else if (game_mode == GameMode::LigaNacoes)
    {
        bool termina(true);
        for (int32 divisao = 0; divisao < liga_das_nacoes.divisoes; ++divisao)
            termina = termina && (liga_das_nacoes.tabelas[divisao].rodada_atual == liga_das_nacoes.tabelas[divisao].calendario.Num());
        if (!termina)
        {
            for (; liga_das_nacoes.grupo_atualiza_tabela < liga_das_nacoes.tabelas.Num(); ++liga_das_nacoes.grupo_atualiza_tabela)
            {
                int32 rodada_atual = liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].rodada_atual;
                int32 jogos = liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].calendario[rodada_atual].jogos.Num();
                for (; liga_das_nacoes.ind_jogo_atualiza_tabela < jogos; ++liga_das_nacoes.ind_jogo_atualiza_tabela)
                {
                    int32 index_t1 = liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].calendario[rodada_atual].jogos[liga_das_nacoes.ind_jogo_atualiza_tabela].casa;
                    int32 index_t2 = liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].calendario[rodada_atual].jogos[liga_das_nacoes.ind_jogo_atualiza_tabela].fora;
                    FResultadoData r;
                    if (liga_das_nacoes.teamMap[index_t1] == team1.index_time)
                    {
                        desempate_por_penaltis = false;
                        team2 = getTeam(index_t2, game_mode);
                        team2.index_time = liga_das_nacoes.teamMap[index_t2];

                        team1_em_casa = true;
                        
                        if (liga_das_nacoes.ind_jogo_atualiza_tabela >= 1)
                            liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].terminaRodada();
                        return true;
                    }
                    else if (liga_das_nacoes.teamMap[index_t2] == team1.index_time)
                    {
                        desempate_por_penaltis = false;
                        team2 = getTeam(index_t1, game_mode);
                        team2.index_time = liga_das_nacoes.teamMap[index_t1];

                        team1_em_casa = false;
                        
                        if (liga_das_nacoes.ind_jogo_atualiza_tabela >= 1)
                            liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].terminaRodada();
                        return true;
                    }
                    else
                    {
                        int32 rodada_final = liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].calendario.Num() - 1;
                        if (rodada_atual != rodada_final)
                        {
                            r = simulaJogo(index_t1, index_t2, false, game_mode);
                            liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].atualizaTabela(index_t1, index_t2, r.gols_casa, r.gols_fora);
                        }
                    }
                }
                liga_das_nacoes.ind_jogo_atualiza_tabela = 0;
                liga_das_nacoes.tabelas[liga_das_nacoes.grupo_atualiza_tabela].terminaRodada();
            }
            liga_das_nacoes.grupo_atualiza_tabela = 0;
            
            // debug
            // for (int32 grupo = 0; grupo < liga_das_nacoes.tabelas.Num(); ++grupo)
            // {
            //     FString JoinedStrRodada("Divisao ");
            //     JoinedStrRodada += FString::FromInt(grupo);
            //     JoinedStrRodada += FString(" Rodada ");
            //     int32 rodada = liga_das_nacoes.tabelas[grupo].rodada_atual - 1;
            //     JoinedStrRodada += FString::FromInt(rodada);
            //     for (int32 indJogo = 0; indJogo < liga_das_nacoes.tabelas[grupo].calendario[rodada].jogos.Num(); ++indJogo)
            //     {
            //         int32 index_casa = liga_das_nacoes.tabelas[grupo].calendario[rodada].jogos[indJogo].casa;
            //         int32 index_fora = liga_das_nacoes.tabelas[grupo].calendario[rodada].jogos[indJogo].fora;
            //         int32 gols_casa = liga_das_nacoes.tabelas[grupo].confrontos.casa[index_casa].fora[index_fora].gols_casa;
            //         int32 gols_fora = liga_das_nacoes.tabelas[grupo].confrontos.casa[index_casa].fora[index_fora].gols_fora;
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
            liga_das_nacoes.terminaTemporada();
            liga_das_nacoes.ind_jogo_atualiza_tabela = 0;
            liga_das_nacoes.grupo_atualiza_tabela = 0;
        }
    }
    return false;
}