// Fill out your copyright notice in the Description page of Project Settings.


#include "PartidaSlotWidget.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UPartidaSlotWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (!FutebasGI)
        return false;

    return true;
}

FSlateBrush UPartidaSlotWidget::bindFlagTudo1()
{
    int32 teams_ind((int32)teams_set);
	if (FutebasGI)
    {
        int32 max_rodadas(1), index_casa(0), index_fora(0), gols_casa(3), gols_fora(3);
        if (game_mode == GameMode::CopaMundo)
        {
            max_rodadas = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].times.Num()-2;
            index_casa = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].casa;
            index_fora = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].fora;
            gols_casa = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_casa;
            gols_fora = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_fora;
        }
        else if (game_mode == GameMode::LigaNacoes)
        {
            max_rodadas = 2*(FutebasGI->GetLiga(teams_ind)->tabelas[grupo].times.Num()-1) - 1;
            index_casa = FutebasGI->GetLiga(teams_ind)->tabelas[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].casa;
            index_fora = FutebasGI->GetLiga(teams_ind)->tabelas[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].fora;
            gols_casa = FutebasGI->GetLiga(teams_ind)->tabelas[grupo].confrontos.casa[index_casa].fora[index_fora].gols_casa;
            gols_fora = FutebasGI->GetLiga(teams_ind)->tabelas[grupo].confrontos.casa[index_casa].fora[index_fora].gols_fora;
        }
        time1Nome->SetText(FText::FromString(FutebasGI->getTeam(index_casa, game_mode, teams_set).nome_hud));
        if (gols_casa >= 0)
            time1Gols->SetText(UKismetTextLibrary::Conv_IntToText(gols_casa));
        else
            time1Gols->SetText(FText::FromString(FString(TEXT(" "))));
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(index_casa, game_mode, teams_set).flag, 32, 24);
    }
	else
    {
        time1Nome->SetText(FText::FromString(FString(TEXT("ERROR"))));
        time1Gols->SetText(UKismetTextLibrary::Conv_IntToText(0));
		return FSlateNoResource();
    }
}

FSlateBrush UPartidaSlotWidget::bindFlagTudo2()
{
    int32 teams_ind((int32)teams_set);
	if (FutebasGI)
    {
        int32 max_rodadas(1), index_casa(0), index_fora(0), gols_casa(3), gols_fora(3);
        if (game_mode == GameMode::CopaMundo)
        {
            max_rodadas = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].times.Num()-2;
            index_casa = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].casa;
            index_fora = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].fora;
            gols_casa = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_casa;
            gols_fora = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_fora;
        }
        else if (game_mode == GameMode::LigaNacoes)
        {
            max_rodadas = 2*(FutebasGI->GetLiga(teams_ind)->tabelas[grupo].times.Num()-1) - 1;
            index_casa = FutebasGI->GetLiga(teams_ind)->tabelas[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].casa;
            index_fora = FutebasGI->GetLiga(teams_ind)->tabelas[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].fora;
            gols_casa = FutebasGI->GetLiga(teams_ind)->tabelas[grupo].confrontos.casa[index_casa].fora[index_fora].gols_casa;
            gols_fora = FutebasGI->GetLiga(teams_ind)->tabelas[grupo].confrontos.casa[index_casa].fora[index_fora].gols_fora;
        }

        time2Nome->SetText(FText::FromString(FutebasGI->getTeam(index_fora, game_mode, teams_set).nome_hud));
        if (gols_fora >= 0)
            time2Gols->SetText(UKismetTextLibrary::Conv_IntToText(gols_fora));
        else
            time2Gols->SetText(FText::FromString(FString(TEXT(" "))));
		return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(index_fora, game_mode, teams_set).flag, 32, 24);
    }
	else
    {
        time2Nome->SetText(FText::FromString(FString(TEXT("ERROR"))));
        time2Gols->SetText(UKismetTextLibrary::Conv_IntToText(0));
		return FSlateNoResource();
    }
}