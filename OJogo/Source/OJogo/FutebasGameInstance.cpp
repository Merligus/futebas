// Fill out your copyright notice in the Description page of Project Settings.


#include "FutebasGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

void UFutebasGameInstance::loadTeams(int32 grupos)
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
        sortear(grupos);

        for (int32 i = 0; i < grupos; ++i)
            tabelaGrupos.Add(FCampeonatoData(teamsArray.Num()/grupos));
    }
}

void UFutebasGameInstance::sortear(int32 grupos)
{
    TArray<TArray<int32>> potes;
    int32 times_por_grupo;
    times_por_grupo = UKismetMathLibrary::FTrunc(teamsArray.Num()/grupos);

    for (int32 i = 0; i < times_por_grupo; ++i)
    {
        TArray<int32> aux;
        while (aux.Num() > 0)
            aux.RemoveAt(0);
        for (int32 j = i*grupos; j < (i+1)*grupos; ++j)
            aux.Add(j);
        potes.Emplace(aux);
    }

    int32 time_sorteado;
    for (int32 i = 0; i < grupos; ++i)
    {
        for (int32 j = 0; j < times_por_grupo; ++j)
        {
            time_sorteado = FMath::RandRange(0, potes[j].Num()-1);
            sorteioGrupo.Add(i*times_por_grupo + j, potes[j][time_sorteado]);
            potes[j].RemoveAt(time_sorteado);
        }
    }
    int32 qatarIndex = *(sorteioGrupo.FindKey(0));
    int32 timeCabecaGrupoA = sorteioGrupo[0];
    sorteioGrupo[0] = 0; // cabeca grupo A precisa ser o Qatar
    sorteioGrupo[qatarIndex] = timeCabecaGrupoA; // o cabeca de grupo que o qatar estava deve ser substituido pelo antigo cabeca do grupo A
}

FTeamData UFutebasGameInstance::getTeam(int32 index)
{
    if (index < teamsArray.Num())
        return teamsArray[sorteioGrupo[index]];
    else
        return FTeamData();
}

void UFutebasGameInstance::bindIndexTimeGrupos()
{
    int32 grupoAtual, posicaoInicial;
    int32 times_por_grupo = tabelaGrupos[0].n_times;

    for (int32 indexSlot = 0; indexSlot < teamsArray.Num(); ++indexSlot)
    {
        grupoAtual = indexSlot/times_por_grupo;
        posicaoInicial = indexSlot % times_por_grupo;

        tabelaGrupos[grupoAtual].adicionaTime(indexSlot, posicaoInicial);
    }

    for (int32 grupo = 0; grupo < tabelaGrupos.Num(); ++grupo)
    {
        tabelaGrupos[grupo].montaConfrontos();

        // debug
        FString JoinedStrRodada("Grupo ");
        JoinedStrRodada += FString::FromInt(grupo);
        for (int32 rodada = 0; rodada < tabelaGrupos[grupo].times.Num()-1; ++rodada)
        {
            JoinedStrRodada += FString(" Rodada ");
            JoinedStrRodada += FString::FromInt(rodada);
            for (int32 indJogo = 0; indJogo < tabelaGrupos[grupo].calendario[rodada].jogos.Num(); ++indJogo)
            {
                JoinedStrRodada += TEXT(" (");
                JoinedStrRodada += getTeam(tabelaGrupos[grupo].calendario[rodada].jogos[indJogo].casa).nome_hud;
                JoinedStrRodada += TEXT(",");
                JoinedStrRodada += getTeam(tabelaGrupos[grupo].calendario[rodada].jogos[indJogo].fora).nome_hud;
                JoinedStrRodada += TEXT(")");
            }
        }
        GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, JoinedStrRodada);
    }
}

void UFutebasGameInstance::simulaJogosProximaRodada()
{
    for (int32 grupo = 0; grupo < tabelaGrupos.Num(); ++grupo)
    {
        int32 rodada_atual = tabelaGrupos[grupo].rodada_atual;
        for (int32 ind_jogo = 0; ind_jogo < tabelaGrupos[grupo].calendario[rodada_atual].jogos.Num(); ++ind_jogo)
        {
            int32 index_t1 = tabelaGrupos[grupo].calendario[rodada_atual].jogos[ind_jogo].casa;
            int32 index_t2 = tabelaGrupos[grupo].calendario[rodada_atual].jogos[ind_jogo].fora;
            float overall_t1 = getTeam(index_t1).habilidades.overall();
            float overall_t2 = getTeam(index_t2).habilidades.overall();
            int32 gols_t1 = FMath::RandRange(0, FMath::TruncToInt(5*overall_t1));
            int32 gols_t2 = FMath::RandRange(0, FMath::TruncToInt(5*overall_t2));

            tabelaGrupos[grupo].atualizaTabela(index_t1, index_t2, gols_t1, gols_t2);
        }
        tabelaGrupos[grupo].terminaRodada();
    }

    // debug
    for (int32 grupo = 0; grupo < tabelaGrupos.Num(); ++grupo)
    {
        FString JoinedStrRodada("Grupo ");
        JoinedStrRodada += FString::FromInt(grupo);
        JoinedStrRodada += FString(" Rodada ");
        int32 rodada = tabelaGrupos[grupo].rodada_atual - 1;
        JoinedStrRodada += FString::FromInt(rodada);
        for (int32 indJogo = 0; indJogo < tabelaGrupos[grupo].calendario[rodada].jogos.Num(); ++indJogo)
        {
            int32 index_casa = tabelaGrupos[grupo].calendario[rodada].jogos[indJogo].casa;
            int32 index_fora = tabelaGrupos[grupo].calendario[rodada].jogos[indJogo].fora;
            int32 gols_casa = tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_casa;
            int32 gols_fora = tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_fora;
            JoinedStrRodada += TEXT(" (");
            JoinedStrRodada += getTeam(index_casa).nome_hud;
            JoinedStrRodada += TEXT(" ");
            JoinedStrRodada += FString::FromInt(gols_casa);
            JoinedStrRodada += TEXT(" x ");
            JoinedStrRodada += FString::FromInt(gols_fora);
            JoinedStrRodada += TEXT(" ");
            JoinedStrRodada += getTeam(index_fora).nome_hud;
            JoinedStrRodada += TEXT(")");
        }
        GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, JoinedStrRodada);
    }
}