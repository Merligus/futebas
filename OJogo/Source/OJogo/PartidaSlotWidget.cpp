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
	if (FutebasGI)
    {
        int32 max_rodadas = FutebasGI->copa_do_mundo.tabelaGrupos[grupo].times.Num()-2;
        int32 index_casa = FutebasGI->copa_do_mundo.tabelaGrupos[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].casa;
        int32 index_fora = FutebasGI->copa_do_mundo.tabelaGrupos[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].fora;
        int32 gols_casa = FutebasGI->copa_do_mundo.tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_casa;
        int32 gols_fora = FutebasGI->copa_do_mundo.tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_fora;

        time1Nome->SetText(FText::FromString(FutebasGI->getTeam(index_casa).nome_hud));
        if (gols_casa >= 0)
            time1Gols->SetText(UKismetTextLibrary::Conv_IntToText(gols_casa));
        else
            time1Gols->SetText(FText::FromString(FString(TEXT(" "))));
		return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(index_casa).flag, 32, 24);
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
	if (FutebasGI)
    {
        int32 max_rodadas = FutebasGI->copa_do_mundo.tabelaGrupos[grupo].times.Num()-2;
        int32 index_casa = FutebasGI->copa_do_mundo.tabelaGrupos[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].casa;
        int32 index_fora = FutebasGI->copa_do_mundo.tabelaGrupos[grupo].calendario[FMath::Clamp(rodada-1, 0, max_rodadas)].jogos[index_jogo].fora;
        int32 gols_casa = FutebasGI->copa_do_mundo.tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_casa;
        int32 gols_fora = FutebasGI->copa_do_mundo.tabelaGrupos[grupo].confrontos.casa[index_casa].fora[index_fora].gols_fora;

        time2Nome->SetText(FText::FromString(FutebasGI->getTeam(index_fora).nome_hud));
        if (gols_fora >= 0)
            time2Gols->SetText(UKismetTextLibrary::Conv_IntToText(gols_fora));
        else
            time2Gols->SetText(FText::FromString(FString(TEXT(" "))));
		return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(index_fora).flag, 32, 24);
    }
	else
    {
        time2Nome->SetText(FText::FromString(FString(TEXT("ERROR"))));
        time2Gols->SetText(UKismetTextLibrary::Conv_IntToText(0));
		return FSlateNoResource();
    }
}