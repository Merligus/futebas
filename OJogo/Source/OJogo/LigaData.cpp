// Fill out your copyright notice in the Description page of Project Settings.


#include "LigaData.h"

FLigaData::FLigaData() :
    divisoes(2), times_por_divisao(16), grupo_atualiza_tabela(0), ind_jogo_atualiza_tabela(0)
{
    for (int32 i = 0; i < divisoes; ++i)
        tabelas.Add(FCampeonatoData(times_por_divisao, false));
}

FLigaData::FLigaData(int32 n_times_por_divisao, int32 n_times) :
    divisoes(n_times/n_times_por_divisao), times_por_divisao(n_times_por_divisao), grupo_atualiza_tabela(0), ind_jogo_atualiza_tabela(0)
{
    for (int32 i = 0; i < divisoes; ++i)
        tabelas.Add(FCampeonatoData(times_por_divisao, false));
}

void FLigaData::alocarTimes(TArray<int32> ranking)
{
    int32 n_slots = divisoes * times_por_divisao;

    teamMap.Reset();
    originalRanking.Reset();
    for (int32 i = 0; i < n_slots; ++i)
    {
        teamMap.Add(i, ranking[i]);
        originalRanking.Add(ranking[i]);
    }
}

int32 FLigaData::putTeamAsLast(int32 true_index)
{
    int32 n_slots = divisoes * times_por_divisao;
    int32 new_slot = n_slots - 1;
    int pos = 0; // posicao do time true_index no ranking

    for (int32 i = 0; i < n_slots; ++i)
    {
        teamMap[i] = originalRanking[i];
        if (originalRanking[i] == true_index)
            pos = i;
    }

    if (pos != new_slot) // se o time selecionado ja esta na ultima posicao nao precisa fazer nada
    {
        for (int32 i = pos; i < new_slot; ++i) // desloca os times para cima até o ultimo
            teamMap[i] = teamMap[i+1];
        teamMap[new_slot] = true_index; // coloca o time selecionado na ultima posicao
    }
    return new_slot;
}

void FLigaData::bindIndexTimeDivisoes()
{
    int32 n_slots = divisoes * times_por_divisao;
    int32 divisao_atual, posicao_inicial;

    for (int32 index_slot = 0; index_slot < n_slots; ++index_slot)
    {
        divisao_atual = index_slot / times_por_divisao;
        posicao_inicial = index_slot % times_por_divisao;

        tabelas[divisao_atual].adicionaTime(index_slot, posicao_inicial);
    }

    for (int32 divisao = 0; divisao < tabelas.Num(); ++divisao)
        tabelas[divisao].montaConfrontos();
}

void FLigaData::terminaTemporada()
{
    campeoes.Add(tabelas[0].tabela[0].index_time);

    TArray<FCampeonatoData> tabelas_temp;
    for (int32 i = 0; i < divisoes; ++i)
        tabelas_temp.Add(FCampeonatoData(times_por_divisao, false));

    for (int32 divisao = 0; divisao < tabelas_temp.Num()-1; ++divisao)
    {
        int32 relegado_index_t1 = tabelas[divisao].tabela[tabelas[divisao].tabela.Num()-3].index_time;
        int32 relegado_index_t2 = tabelas[divisao].tabela[tabelas[divisao].tabela.Num()-2].index_time;
        int32 relegado_index_t3 = tabelas[divisao].tabela[tabelas[divisao].tabela.Num()-1].index_time;
        
        int32 acesso_index_t1 = tabelas[divisao+1].tabela[0].index_time;
        int32 acesso_index_t2 = tabelas[divisao+1].tabela[1].index_time;
        int32 acesso_index_t3 = tabelas[divisao+1].tabela[2].index_time;

        int32 initial_team = (divisao > 0)? 3 : 0;
        int32 final_team = times_por_divisao - 3;
        for (int32 t = initial_team; t < final_team; ++t)
            tabelas_temp[divisao].adicionaTime(tabelas[divisao].tabela[t].index_time, t);

        tabelas_temp[divisao].adicionaTime(acesso_index_t1, final_team);
        tabelas_temp[divisao].adicionaTime(acesso_index_t2, final_team+1);
        tabelas_temp[divisao].adicionaTime(acesso_index_t3, final_team+2);

        tabelas_temp[divisao+1].adicionaTime(relegado_index_t1, 0);
        tabelas_temp[divisao+1].adicionaTime(relegado_index_t2, 1);
        tabelas_temp[divisao+1].adicionaTime(relegado_index_t3, 2);
    }

    // ultima divisao
    for (int32 t = 3; t < times_por_divisao; ++t)
        tabelas_temp[divisoes-1].adicionaTime(tabelas[divisoes-1].tabela[t].index_time, t);
    
    for (int32 divisao = 0; divisao < tabelas_temp.Num(); ++divisao)
    {
        tabelas_temp[divisao].montaConfrontos();
        tabelas[divisao] = tabelas_temp[divisao];
    }
}

bool FLigaData::isCampeao(int32 index_time_p) const
{
    int32 index_time = 0;
    for (int32 index_slot = 0; index_slot < teamMap.Num(); ++index_slot)
        if (teamMap[index_slot] == index_time_p)
            index_time = index_slot;

    return campeoes.Find(index_time) != INDEX_NONE;
}