// Fill out your copyright notice in the Description page of Project Settings.


#include "CopaMundoData.h"

FCopaMundoData::FCopaMundoData() :
    fase_atual(0), grupo_atualiza_tabela(0), ind_jogo_atualiza_tabela(0)
{
    for (int32 i = 0; i < 8; ++i)
        tabelaGrupos.Add(FCampeonatoData(4));

    sorteioGrupo.Reset();
    for (int32 i = 0; i < 32; ++i)
        sorteioGrupo.Add(i, i);
}

void FCopaMundoData::sortear(TArray<int32> ranking)
{
    sorteioGrupo.Reset();

    TArray<TArray<int32>> potes;

    for (int32 i = 0; i < ranking.Num(); ++i) // procurar pelo catar
    {
        if (ranking[i] == 0) // se eh o index do catar
        {
            int qatarIndex = ranking[i]; // index do catar
            ranking.RemoveAt(i); // remove o catar da posicao no ranking
            ranking.Insert(qatarIndex, 0); // forca o catar a ser o primeiro do ranking
            break;
        }
    }

    for (int32 i = 0; i < 4; ++i)
    {
        TArray<int32> aux;
        while (aux.Num() > 0)
            aux.RemoveAt(0);
        for (int32 j = i*8; j < (i+1)*8; ++j)
            aux.Add(ranking[j]);
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

int32 FCopaMundoData::getGolsTime(int32 index_time_p) const
{
    int32 soma_gols = 0;
    int32 index_time = 0;
    for (int32 index_slot = 0; index_slot < sorteioGrupo.Num(); ++index_slot)
        if (sorteioGrupo[index_slot] == index_time_p)
            index_time = index_slot;
            
    for (int32 grupo = 0; grupo < tabelaGrupos.Num() && soma_gols == 0; ++grupo)
        for (int32 time = 0; time < tabelaGrupos[grupo].tabela.Num() && soma_gols == 0; ++time)
            if (tabelaGrupos[grupo].tabela[time].index_time == index_time)
                soma_gols = tabelaGrupos[grupo].tabela[time].gols_pro;

    for (int32 fase = 0; fase < faseFinal.fases.Num(); ++fase)
    {
        for (int32 confronto = 0; confronto < faseFinal.fases[fase].confrontos.Num(); ++confronto)
        {
            if (faseFinal.fases[fase].confrontos[confronto].index_casa == index_time)
                if (faseFinal.fases[fase].confrontos[confronto].gols_casa >= 0)
                    soma_gols += faseFinal.fases[fase].confrontos[confronto].gols_casa;
            if (faseFinal.fases[fase].confrontos[confronto].index_fora == index_time)
                if (faseFinal.fases[fase].confrontos[confronto].gols_fora >= 0)
                    soma_gols += faseFinal.fases[fase].confrontos[confronto].gols_fora;
        }
    }
    return soma_gols;
}

bool FCopaMundoData::isCampeao(int32 index_time_p) const
{
    if (fase_atual >= 4)
    {
        int32 index_time = 0;
        for (int32 index_slot = 0; index_slot < sorteioGrupo.Num(); ++index_slot)
            if (sorteioGrupo[index_slot] == index_time_p)
                index_time = index_slot;
        
        return index_time == faseFinal.fases[3].confrontos[0].getGanhador();
    }
    else
        return false;
}