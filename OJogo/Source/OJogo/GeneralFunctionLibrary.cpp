// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FutebasSaveGame.h"

void UGeneralFunctionLibrary::loadGame(const UObject* WorldContextObject, const GameMode game_mode, UFutebasGameInstance* FutebasGI)
{
    if (UGameplayStatics::DoesSaveGameExist(FString(TEXT("FutebasSavedGame")), 0))
    {
        UFutebasSaveGame* FutebasSG;
        FutebasSG = Cast<UFutebasSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(TEXT("FutebasSavedGame")), 0));
        
        FutebasGI->copa_do_mundo = FutebasSG->copa_do_mundo;
        FutebasGI->liga_das_nacoes = FutebasSG->liga_das_nacoes;

        if (game_mode == GameMode::CopaMundo)
        {
            if (FutebasSG->copa_do_mundo.team1.index_time >= 0)
            {
                FutebasGI->team1 = FutebasSG->copa_do_mundo.team1;
                UGameplayStatics::OpenLevel(WorldContextObject, FName(TEXT("Copa_Level")));
            }
        }
        else if (game_mode == GameMode::LigaNacoes)
        {
            if (FutebasSG->liga_das_nacoes.team1.index_time >= 0)
            {
                FutebasGI->team1 = FutebasSG->liga_das_nacoes.team1;
                UGameplayStatics::OpenLevel(WorldContextObject, FName(TEXT("Liga_Nacoes_Level")));
            }
        }
    }
}

void UGeneralFunctionLibrary::saveGame(const GameMode game_mode, const FCopaMundoData CdM, const FLigaData LdN, const FTeamData team1)
{
    UFutebasSaveGame* FutebasSG;
    if (UGameplayStatics::DoesSaveGameExist(FString(TEXT("FutebasSavedGame")), 0))
        FutebasSG = Cast<UFutebasSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(TEXT("FutebasSavedGame")), 0));
    else
        FutebasSG = Cast<UFutebasSaveGame>(UGameplayStatics::CreateSaveGameObject(UFutebasSaveGame::StaticClass()));
    
    if (game_mode == GameMode::CopaMundo)
    {
        FutebasSG->copa_do_mundo = CdM;
        FutebasSG->copa_do_mundo.team1 = team1;
    }
    else if (game_mode == GameMode::LigaNacoes)
    {
        FutebasSG->liga_das_nacoes = LdN;
        FutebasSG->liga_das_nacoes.team1 = team1;
    }
    UGameplayStatics::SaveGameToSlot(FutebasSG, FString(TEXT("FutebasSavedGame")), 0);
}