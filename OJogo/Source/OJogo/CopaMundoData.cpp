// Fill out your copyright notice in the Description page of Project Settings.


#include "CopaMundoData.h"

FCopaMundoData::FCopaMundoData() :
    fase_atual(0)
{
    for (int32 i = 0; i < 8; ++i)
        tabelaGrupos.Add(FCampeonatoData(4));
}

void FCopaMundoData::sortear()
{
    TArray<TArray<int32>> potes;

    for (int32 i = 0; i < 4; ++i)
    {
        TArray<int32> aux;
        while (aux.Num() > 0)
            aux.RemoveAt(0);
        for (int32 j = i*8; j < (i+1)*8; ++j)
            aux.Add(j);
        potes.Emplace(aux);
    }

    int32 time_sorteado;
    for (int32 i = 0; i < 8; ++i)
    {
        for (int32 j = 0; j < 4; ++j)
        {
            time_sorteado = FMath::RandRange(0, potes[j].Num()-1);
            sorteioGrupo.Add(i*4 + j, potes[j][time_sorteado]);
            potes[j].RemoveAt(time_sorteado);
        }
    }
    int32 qatarIndex = *(sorteioGrupo.FindKey(0));
    int32 timeCabecaGrupoA = sorteioGrupo[0];
    sorteioGrupo[0] = 0; // cabeca grupo A precisa ser o Qatar
    sorteioGrupo[qatarIndex] = timeCabecaGrupoA; // o cabeca de grupo que o qatar estava deve ser substituido pelo antigo cabeca do grupo A
}

void FCopaMundoData::bindIndexTimeGrupos()
{
    int32 grupoAtual, posicaoInicial;
    int32 times_por_grupo = tabelaGrupos[0].n_times;

    for (int32 indexSlot = 0; indexSlot < 32; ++indexSlot)
    {
        grupoAtual = indexSlot/times_por_grupo;
        posicaoInicial = indexSlot % times_por_grupo;

        tabelaGrupos[grupoAtual].adicionaTime(indexSlot, posicaoInicial);
    }

    for (int32 grupo = 0; grupo < tabelaGrupos.Num(); ++grupo)
    {
        tabelaGrupos[grupo].montaConfrontos();

        // debug
        // FString JoinedStrRodada("Grupo ");
        // JoinedStrRodada += FString::FromInt(grupo);
        // for (int32 rodada = 0; rodada < tabelaGrupos[grupo].times.Num()-1; ++rodada)
        // {
        //     JoinedStrRodada += FString(" Rodada ");
        //     JoinedStrRodada += FString::FromInt(rodada);
        //     for (int32 indJogo = 0; indJogo < tabelaGrupos[grupo].calendario[rodada].jogos.Num(); ++indJogo)
        //     {
        //         JoinedStrRodada += TEXT(" (");
        //         JoinedStrRodada += getTeam(tabelaGrupos[grupo].calendario[rodada].jogos[indJogo].casa).nome_hud;
        //         JoinedStrRodada += TEXT(",");
        //         JoinedStrRodada += getTeam(tabelaGrupos[grupo].calendario[rodada].jogos[indJogo].fora).nome_hud;
        //         JoinedStrRodada += TEXT(")");
        //     }
        // }
        // GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, JoinedStrRodada);
    }
}

void FCopaMundoData::chaveia()
{
    fase_atual += 1;
    if (fase_atual == 1) // oitavas
    {
        faseFinal.chaveia(fase_atual);
        for (int32 par_grupo = 0; par_grupo < 4; ++par_grupo)
        {
            int32 primeiro_colocado1 = tabelaGrupos[2*par_grupo].tabela[0].index_time;
            int32 segundo_colocado1 = tabelaGrupos[2*par_grupo].tabela[1].index_time;

            int32 primeiro_colocado2 = tabelaGrupos[2*par_grupo+1].tabela[0].index_time;
            int32 segundo_colocado2 = tabelaGrupos[2*par_grupo+1].tabela[1].index_time;

            faseFinal.fases[0].confrontos[par_grupo].index_casa = primeiro_colocado1;
            faseFinal.fases[0].confrontos[par_grupo].index_fora = segundo_colocado2;

            faseFinal.fases[0].confrontos[par_grupo+4].index_casa = primeiro_colocado2;
            faseFinal.fases[0].confrontos[par_grupo+4].index_fora = segundo_colocado1;
        }
    }
    else if (fase_atual <= 4)
    {
        faseFinal.chaveia(fase_atual);
        for (int32 jogo = 0; jogo < faseFinal.fases[fase_atual-2].confrontos.Num()/2; ++jogo)
        {
            int32 classificado1 = faseFinal.fases[fase_atual-2].confrontos[2*jogo].getGanhador();
            int32 classificado2 = faseFinal.fases[fase_atual-2].confrontos[2*jogo+1].getGanhador();

            faseFinal.fases[fase_atual-1].confrontos[jogo].index_casa = classificado1;
            faseFinal.fases[fase_atual-1].confrontos[jogo].index_fora = classificado2;
        }
    }
}