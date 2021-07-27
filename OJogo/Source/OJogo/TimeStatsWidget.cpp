// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeStatsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "GeneralFunctionLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "FutebasSaveGame.h"

bool UTimeStatsWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
    return ReiniciarModo();
}

bool UTimeStatsWidget::ReiniciarModo()
{
    int32 teams_ind((int32)teams_set);
    if (FutebasGI)
    {
        FutebasGI->loadTeams();
        if (game_mode == GameMode::CopaMundo)
        {
            FutebasGI->SetCopa(FCopaMundoData(), teams_ind);
            FutebasGI->GetCopa(teams_ind)->sortear();
        }
        else if (game_mode == GameMode::LigaNacoes)
        {
            FutebasGI->SetLiga(FLigaData(16, 32), teams_ind);
            FutebasGI->GetLiga(teams_ind)->alocarTimes();
        }
        FutebasGI->team1 = FTeamData();
        FutebasGI->team2 = FTeamData();
        return true;
    }
    else
        return false;
}

void UTimeStatsWidget::sorteiaGruposCopa()
{
    int32 teams_ind((int32)teams_set);
    if (FutebasGI)
        FutebasGI->GetCopa(teams_ind)->sortear();
    if (FutebasGI->team1.index_time >= 0)
        FutebasGI->team1_index_slot = *(FutebasGI->GetCopa(teams_ind)->sorteioGrupo.FindKey(FutebasGI->team1.index_time));
}

bool UTimeStatsWidget::bindStatsEnabled(bool team1)
{
    if (FutebasGI)
    {
        if (team1)
            return FutebasGI->team1.index_time >= 0;
        else
            return FutebasGI->team2.index_time >= 0;
    }
    else
        return false;
}

FSlateBrush UTimeStatsWidget::bindFlag(bool team1)
{
    if (FutebasGI)
    {
        if (team1)
        {
            if (FutebasGI->team1.index_time >= 0)
                return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->team1.flag, 64, 48);
            else
                return FSlateNoResource();
        }
        else
        {
            if (FutebasGI->team2.index_time >= 0)
                return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->team2.flag, 64, 48);
            else
                return FSlateNoResource();
        }
    }
    else
        return FSlateNoResource();
}

FText UTimeStatsWidget::bindTeamName(bool team1)
{
    if (FutebasGI)
    {
        if (team1)
            return UKismetTextLibrary::Conv_StringToText(FutebasGI->team1.nome_hud);
        else
            return UKismetTextLibrary::Conv_StringToText(FutebasGI->team2.nome_hud);
    }
    else
        return FText::FromString(FString("Time"));
}

float UTimeStatsWidget::bindChute(bool team1)
{
    if (FutebasGI)
    {
        if (team1)
            return FutebasGI->team1.habilidades.maxForcaChute;
        else
            return FutebasGI->team2.habilidades.maxForcaChute;
    }
    else
        return 0.0f;
}

float UTimeStatsWidget::bindVelocidade(bool team1)
{
    if (FutebasGI)
    {
        if (team1)
            return FutebasGI->team1.habilidades.velocidade;
        else
            return FutebasGI->team2.habilidades.velocidade;
    }
    else
        return 0.0f;
}

float UTimeStatsWidget::bindCarrinho(bool team1)
{
    if (FutebasGI)
    {
        if (team1)
            return FutebasGI->team1.habilidades.velocidadeCarrinho;
        else
            return FutebasGI->team2.habilidades.velocidadeCarrinho;
    }
    else
        return 0.0f;
}

float UTimeStatsWidget::bindEnergia(bool team1)
{
    if (FutebasGI)
    {
        if (team1)
            return FutebasGI->team1.habilidades.staminaRegen;
        else
            return FutebasGI->team2.habilidades.staminaRegen;
    }
    else
        return 0.0f;
}

void UTimeStatsWidget::bindIndexTimeGrupos()
{
    int32 teams_ind((int32)teams_set);
    if (FutebasGI)
    {
        if (game_mode == GameMode::CopaMundo)
            FutebasGI->GetCopa(teams_ind)->bindIndexTimeGrupos();
        else if (game_mode == GameMode::LigaNacoes)
        {
            FutebasGI->GetLiga(teams_ind)->bindIndexTimeDivisoes();
            // debug
            // for (int32 divisao = 0; divisao < FutebasGI->GetLiga(teams_ind)->tabelas.Num(); ++divisao)
            // {
            //     FString JoinedStrRodada("Divisao ");
            //     JoinedStrRodada += FString::FromInt(divisao);
            //     for (int32 rodada = 0; rodada < FutebasGI->GetLiga(teams_ind)->tabelas[divisao].times.Num()-1; ++rodada)
            //     {
            //         JoinedStrRodada += FString(" Rodada ");
            //         JoinedStrRodada += FString::FromInt(rodada);
            //         for (int32 indJogo = 0; indJogo < FutebasGI->GetLiga(teams_ind)->tabelas[divisao].calendario[rodada].jogos.Num(); ++indJogo)
            //         {
            //             JoinedStrRodada += TEXT(" (");
            //             JoinedStrRodada += FutebasGI->getTeam(FutebasGI->GetLiga(teams_ind)->tabelas[divisao].calendario[rodada].jogos[indJogo].casa, game_mode).nome_hud;
            //             JoinedStrRodada += TEXT(",");
            //             JoinedStrRodada += FutebasGI->getTeam(FutebasGI->GetLiga(teams_ind)->tabelas[divisao].calendario[rodada].jogos[indJogo].fora, game_mode).nome_hud;
            //             JoinedStrRodada += TEXT(")");
            //         }
            //     }
            //     GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, JoinedStrRodada);
            // }
        }
        FutebasGI->current_game_mode = game_mode;
        FutebasGI->current_teams_set = teams_set;
        UGeneralFunctionLibrary::saveGame(*FutebasGI->GetCopa(teams_ind), *FutebasGI->GetLiga(teams_ind), FutebasGI->team1, game_mode, teams_set);
    }
}

void UTimeStatsWidget::loadGame()
{
    if (FutebasGI)
    {
        FutebasGI->current_game_mode = game_mode;
        FutebasGI->current_teams_set = teams_set;
        UGeneralFunctionLibrary::loadGame(GetWorld(), FutebasGI, game_mode, teams_set);
    }
}