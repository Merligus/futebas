// Fill out your copyright notice in the Description page of Project Settings.


#include "FutebasGameInstance.h"

void UFutebasGameInstance::loadTeams()
{
    FString Context;
    TArray<FTeamData*> aux;

    teams->GetAllRows(Context, aux);

    for (int32 i = 0; i < aux.Num(); ++i)
        teamsArray.Add(*(aux[i]));
}

FTeamData UFutebasGameInstance::getTeam(int32 index)
{
    if (index < teamsArray.Num())
        return teamsArray[index];
    else
        return FTeamData();
}