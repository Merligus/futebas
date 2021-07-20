// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeStatsWidget.h"
#include "Kismet/KismetTextLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UTimeStatsWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (FutebasGI)
    {
        FutebasGI->loadTeams();
        FutebasGI->copa_do_mundo = FCopaMundoData();
        FutebasGI->copa_do_mundo.sortear();
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
        FutebasGI->copa_do_mundo.bindIndexTimeGrupos();
}