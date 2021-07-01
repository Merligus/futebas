// Fill out your copyright notice in the Description page of Project Settings.


#include "FutebasGameInstance.h"

FTeamData UFutebasGameInstance::getTeam(int32 index)
{
    FString Context;
    TArray<FTeamData*> teamsArray;

    teams->GetAllRows(Context, teamsArray);
    if (index < teamsArray.Num())
        return *(teamsArray[index]);
    else
        return FTeamData();
}