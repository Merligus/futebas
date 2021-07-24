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
	if (FutebasGI)
    {
        FutebasGI->loadTeams();
        if (game_mode == GameMode::CopaMundo)
        {
            FutebasGI->copa_do_mundo = FCopaMundoData();
            FutebasGI->copa_do_mundo.sortear();
        }
        else if (game_mode == GameMode::LigaNacoes)
        {
            FutebasGI->liga_das_nacoes = FLigaData(16, 32);
            FutebasGI->liga_das_nacoes.alocarTimes();
        }
        FutebasGI->team1 = FTeamData();
        FutebasGI->team2 = FTeamData();
    }
    else
        return false;

    return true;
}

void UTimeStatsWidget::sorteiaGruposCopa()
{
    if (FutebasGI)
        FutebasGI->copa_do_mundo.sortear();
    FutebasGI->team1_index_slot = *(FutebasGI->copa_do_mundo.sorteioGrupo.FindKey(FutebasGI->team1.index_time));
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
    if (FutebasGI)
    {
        if (game_mode == GameMode::CopaMundo)
            FutebasGI->copa_do_mundo.bindIndexTimeGrupos();
        else if (game_mode == GameMode::LigaNacoes)
        {
            FutebasGI->liga_das_nacoes.bindIndexTimeDivisoes();
            // debug
            // for (int32 divisao = 0; divisao < FutebasGI->liga_das_nacoes.tabelas.Num(); ++divisao)
            // {
            //     FString JoinedStrRodada("Divisao ");
            //     JoinedStrRodada += FString::FromInt(divisao);
            //     for (int32 rodada = 0; rodada < FutebasGI->liga_das_nacoes.tabelas[divisao].times.Num()-1; ++rodada)
            //     {
            //         JoinedStrRodada += FString(" Rodada ");
            //         JoinedStrRodada += FString::FromInt(rodada);
            //         for (int32 indJogo = 0; indJogo < FutebasGI->liga_das_nacoes.tabelas[divisao].calendario[rodada].jogos.Num(); ++indJogo)
            //         {
            //             JoinedStrRodada += TEXT(" (");
            //             JoinedStrRodada += FutebasGI->getTeam(FutebasGI->liga_das_nacoes.tabelas[divisao].calendario[rodada].jogos[indJogo].casa, game_mode).nome_hud;
            //             JoinedStrRodada += TEXT(",");
            //             JoinedStrRodada += FutebasGI->getTeam(FutebasGI->liga_das_nacoes.tabelas[divisao].calendario[rodada].jogos[indJogo].fora, game_mode).nome_hud;
            //             JoinedStrRodada += TEXT(")");
            //         }
            //     }
            //     GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Red, JoinedStrRodada);
            // }
        }
        UGeneralFunctionLibrary::saveGame(game_mode, FutebasGI->copa_do_mundo, FutebasGI->liga_das_nacoes, FutebasGI->team1);
    }
}

void UTimeStatsWidget::loadGame()
{
    UGeneralFunctionLibrary::loadGame(GetWorld(), game_mode, FutebasGI);
}