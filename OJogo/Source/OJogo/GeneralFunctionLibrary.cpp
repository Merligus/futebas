// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FutebasSaveGame.h"

void UGeneralFunctionLibrary::loadGame(const UObject* WorldContextObject, UFutebasGameInstance* FutebasGI, const GameMode game_mode, const TeamsSet teams_set)
{
    int32 teams_ind((int32)teams_set);
    if (UGameplayStatics::DoesSaveGameExist(FString(TEXT("FutebasSavedGame")), 0))
    {
        UFutebasSaveGame* FutebasSG;
        FutebasSG = Cast<UFutebasSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(TEXT("FutebasSavedGame")), 0));

        FutebasGI->SetCopa(*FutebasSG->GetCopa(teams_ind), teams_ind);
        FutebasGI->SetLiga(*FutebasSG->GetLiga(teams_ind), teams_ind);
        if (game_mode == GameMode::CopaMundo)
        {
            if (FutebasSG->GetCopa(teams_ind)->team1.index_time >= 0)
            {
                FutebasGI->team1 = FutebasSG->GetCopa(teams_ind)->team1;
                UGameplayStatics::OpenLevel(WorldContextObject, FName(TEXT("Copa_Level")));
            }
        }
        else if (game_mode == GameMode::LigaNacoes)
        {
            if (FutebasSG->GetLiga(teams_ind)->team1.index_time >= 0)
            {
                FutebasGI->team1 = FutebasSG->GetLiga(teams_ind)->team1;
                UGameplayStatics::OpenLevel(WorldContextObject, FName(TEXT("Liga_Nacoes_Level")));
            }
        }
    }
}

void UGeneralFunctionLibrary::saveGame(const FCopaMundoData CdM, const FLigaData LdN, const FTeamData team1, const GameMode game_mode, const TeamsSet teams_set)
{
    int32 teams_ind((int32)teams_set);
    UFutebasSaveGame* FutebasSG;
    if (UGameplayStatics::DoesSaveGameExist(FString(TEXT("FutebasSavedGame")), 0))
        FutebasSG = Cast<UFutebasSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(TEXT("FutebasSavedGame")), 0));
    else
        FutebasSG = Cast<UFutebasSaveGame>(UGameplayStatics::CreateSaveGameObject(UFutebasSaveGame::StaticClass()));
    
    if (game_mode == GameMode::CopaMundo)
    {
        FutebasSG->SetCopa(CdM, teams_ind);
        FutebasSG->GetCopa(teams_ind)->team1 = team1;
    }
    else if (game_mode == GameMode::LigaNacoes)
    {
        FutebasSG->SetLiga(LdN, teams_ind);
        FutebasSG->GetLiga(teams_ind)->team1 = team1;
    }
    UGameplayStatics::SaveGameToSlot(FutebasSG, FString(TEXT("FutebasSavedGame")), 0);
}