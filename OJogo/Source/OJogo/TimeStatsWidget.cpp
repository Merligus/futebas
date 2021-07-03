// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeStatsWidget.h"
#include "Kismet/KismetTextLibrary.h"


bool UTimeStatsWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (FutebasGI)
    {
        FutebasGI->loadTeams(8);
        FutebasGI->team1 = FTeamData();
    }
    else
        return false;

    return true;
}

void UTimeStatsWidget::sorteiaGruposCopa()
{
    if (FutebasGI)
        FutebasGI->sortear(8);
}

bool UTimeStatsWidget::bindStatsEnabled()
{
    if (FutebasGI)
        return FutebasGI->team1.nome_hud.Len() > 0;
    else
        return false;
}

FText UTimeStatsWidget::bindTeamName()
{
    if (FutebasGI)
        return UKismetTextLibrary::Conv_StringToText(FutebasGI->team1.nome_hud);
    else
        return FText::FromString(FString("Time"));
}

float UTimeStatsWidget::bindChute()
{
    if (FutebasGI)
        return FutebasGI->team1.habilidades.maxForcaChute;
    else
        return 0.0f;
}

float UTimeStatsWidget::bindVelocidade()
{
    if (FutebasGI)
        return FutebasGI->team1.habilidades.velocidade;
    else
        return 0.0f;
}

float UTimeStatsWidget::bindCarrinho()
{
    if (FutebasGI)
        return FutebasGI->team1.habilidades.velocidadeCarrinho;
    else
        return 0.0f;
}

float UTimeStatsWidget::bindEnergia()
{
    if (FutebasGI)
        return FutebasGI->team1.habilidades.staminaRegen;
    else
        return 0.0f;
}