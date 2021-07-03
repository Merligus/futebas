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
    int32 grupoAtual, posicaoAtual;
    int32 times_por_grupo = tabelaGrupos[0].n_times;

    for (int32 indexSlot = 0; indexSlot < teamsArray.Num(); ++indexSlot)
    {
        grupoAtual = indexSlot/times_por_grupo;
        posicaoAtual = indexSlot % times_por_grupo;

        tabelaGrupos[grupoAtual].tabela[posicaoAtual].index_time = indexSlot;
    }
}