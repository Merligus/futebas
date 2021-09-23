// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamSlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UTeamSlotWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (!FutebasGI)
        return false;
    FutebasGI->escolheTeam2 = false;

    if (!ensure(timeBotao != nullptr))
        return false;

    if (game_mode == GameMode::NovaPartida)
        timeBotao->OnClicked.AddDynamic(this, &UTeamSlotWidget::ButtonClickedNovaPartida);
    else if (game_mode == GameMode::NovaPartidaOnline)
        timeBotao->OnClicked.AddDynamic(this, &UTeamSlotWidget::ButtonClickedNovaPartidaOnline);
    else
        timeBotao->OnClicked.AddDynamic(this, &UTeamSlotWidget::ButtonClicked);

    return true;
}

void UTeamSlotWidget::ButtonClicked()
{
    int32 teams_ind((int32)teams_set);
    if (FutebasGI)
    {
        FutebasGI->team1 = FutebasGI->getTeam(indexSlot, game_mode, teams_set);
        FutebasGI->team1_index_slot = indexSlot;
        FutebasGI->team2 = FutebasGI->getTeam(indexSlot, game_mode, teams_set);
        FutebasGI->team2_index_slot = -1;
        if (game_mode == GameMode::LigaNacoes)
            FutebasGI->team1_index_slot = FutebasGI->GetLiga(teams_ind)->putTeamAsLast(FutebasGI->team1.index_time);
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("Slot clicado mas FutebasGI null"));
}

void UTeamSlotWidget::ButtonClickedNovaPartida()
{
    if (FutebasGI)
    {
        if (FutebasGI->escolheTeam2)
        {
            FutebasGI->team2 = FutebasGI->getTeamTrueIndex(indexSlot, teams_set);
            FutebasGI->team2_index_slot = indexSlot;
        }
        else
        {
            FutebasGI->team1 = FutebasGI->getTeamTrueIndex(indexSlot, teams_set);
            FutebasGI->team1_index_slot = indexSlot;
        }
        FutebasGI->escolheTeam2 = !FutebasGI->escolheTeam2;
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("Slot clicado mas FutebasGI null"));
}

void UTeamSlotWidget::ButtonClickedNovaPartidaOnline()
{
    if (FutebasGI)
    {
        FutebasGI->team1 = FutebasGI->getTeamTrueIndex(indexSlot, teams_set);
        FutebasGI->team1_index_slot = indexSlot;
        FutebasGI->team2 = FutebasGI->getTeamTrueIndex(indexSlot, teams_set);
        FutebasGI->team2_index_slot = indexSlot;
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("Slot clicado mas FutebasGI null"));
}

FSlateBrush UTeamSlotWidget::bindFlagNome()
{
	if (FutebasGI)
    {
        timeNome->SetText(FText::FromString(FutebasGI->getTeam(indexSlot, game_mode, teams_set).nome_hud));
		return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(indexSlot, game_mode, teams_set).flag, 32, 24);
    }
	else
    {
        timeNome->SetText(FText::FromString(FString(TEXT("ERROR"))));
		return FSlateNoResource();
    }
}

FSlateBrush UTeamSlotWidget::bindFlagNomeNovaPartida()
{
	if (FutebasGI)
    {
        timeNome->SetText(FText::FromString(FutebasGI->getTeamTrueIndex(indexSlot, teams_set).nome_hud));
		return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeamTrueIndex(indexSlot, teams_set).flag, 32, 24);
    }
	else
    {
        timeNome->SetText(FText::FromString(FString(TEXT("ERROR"))));
		return FSlateNoResource();
    }
}

FLinearColor UTeamSlotWidget::bindSelected()
{
    FLinearColor c;
    if (indexSlot == FutebasGI->team1_index_slot && FutebasGI->team1.index_time >= 0)
    {
        if (indexSlot == FutebasGI->team2_index_slot && FutebasGI->team2.index_time >= 0)
            c = FLinearColor(1, 0, 1, 0.5);
        else
            c = FLinearColor(0, 0, 1, 0.5);
    }
    else if (indexSlot == FutebasGI->team2_index_slot && FutebasGI->team2.index_time >= 0 && game_mode == GameMode::NovaPartida)
        c = FLinearColor(1, 0, 0, 0.5);
    else
        c = FLinearColor(0, 0, 0, 0);
    
    return c;
}