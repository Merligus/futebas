// Fill out your copyright notice in the Description page of Project Settings.


#include "PartidaMataMataSlotWidget.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UPartidaMataMataSlotWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (!FutebasGI)
        return false;

    return true;
}

FSlateBrush UPartidaMataMataSlotWidget::bindFlagTudo1()
{
    int32 teams_ind((int32)teams_set);
	if (FutebasGI)
    {
        int32 fase_atual = FutebasGI->GetCopa(teams_ind)->fase_atual;
        if (fase <= fase_atual)
        {
            int32 index_casa = FutebasGI->GetCopa(teams_ind)->faseFinal.fases[fase-1].confrontos[index_jogo].index_casa;
            int32 gols_casa = FutebasGI->GetCopa(teams_ind)->faseFinal.fases[fase-1].confrontos[index_jogo].gols_casa;
            int32 gols_penalti_casa = FutebasGI->GetCopa(teams_ind)->faseFinal.fases[fase-1].confrontos[index_jogo].gols_penalti_casa;

            time1Nome->SetText(FText::FromString(FutebasGI->getTeam(index_casa, GameMode::CopaMundo, teams_set).nome_hud));
            if (gols_casa >= 0)
                time1Gols->SetText(UKismetTextLibrary::Conv_IntToText(gols_casa));
            else
                time1Gols->SetText(FText::FromString(FString(TEXT(" "))));
            if (gols_penalti_casa >= 0)
                time1GolsPen->SetText(FText::FromString(FString(TEXT("(")) + FString::FromInt(gols_penalti_casa) + FString(TEXT(")")) ));
            else
                time1GolsPen->SetText(FText::FromString(FString(TEXT(""))));            
            return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(index_casa, GameMode::CopaMundo, teams_set).flag, 24, 18);
        }
        else
        {
            time1Nome->SetText(FText::FromString(FString(TEXT(""))));
            time1Gols->SetText(FText::FromString(FString(TEXT(""))));
            time1GolsPen->SetText(FText::FromString(FString(TEXT(""))));
            return FSlateNoResource();
        }
    }
	else
    {
        time1Nome->SetText(FText::FromString(FString(TEXT("ERROR"))));
        time1Gols->SetText(FText::FromString(FString(TEXT(""))));
        time1GolsPen->SetText(FText::FromString(FString(TEXT(""))));
		return FSlateNoResource();
    }
}

FSlateBrush UPartidaMataMataSlotWidget::bindFlagTudo2()
{
    int32 teams_ind((int32)teams_set);
	if (FutebasGI)
    {
        int32 fase_atual = FutebasGI->GetCopa(teams_ind)->fase_atual;
        if (fase <= fase_atual)
        {
            int32 index_fora = FutebasGI->GetCopa(teams_ind)->faseFinal.fases[fase-1].confrontos[index_jogo].index_fora;
            int32 gols_fora = FutebasGI->GetCopa(teams_ind)->faseFinal.fases[fase-1].confrontos[index_jogo].gols_fora;
            int32 gols_penalti_fora = FutebasGI->GetCopa(teams_ind)->faseFinal.fases[fase-1].confrontos[index_jogo].gols_penalti_fora;

            time2Nome->SetText(FText::FromString(FutebasGI->getTeam(index_fora, GameMode::CopaMundo, teams_set).nome_hud));
            if (gols_fora >= 0)
                time2Gols->SetText(UKismetTextLibrary::Conv_IntToText(gols_fora));
            else
                time2Gols->SetText(FText::FromString(FString(TEXT(" "))));
            if (gols_penalti_fora >= 0)
                time2GolsPen->SetText(FText::FromString(FString(TEXT("(")) + FString::FromInt(gols_penalti_fora) + FString(TEXT(")")) ));
            else
                time2GolsPen->SetText(FText::FromString(FString(TEXT(""))));            
            return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(index_fora, GameMode::CopaMundo, teams_set).flag, 24, 18);
        }
        else
        {
            time2Nome->SetText(FText::FromString(FString(TEXT(""))));
            time2Gols->SetText(FText::FromString(FString(TEXT(""))));
            time2GolsPen->SetText(FText::FromString(FString(TEXT(""))));
            return FSlateNoResource();
        }
    }
	else
    {
        time2Nome->SetText(FText::FromString(FString(TEXT("ERROR"))));
        time2Gols->SetText(FText::FromString(FString(TEXT(""))));
        time2GolsPen->SetText(FText::FromString(FString(TEXT(""))));
		return FSlateNoResource();
    }
}