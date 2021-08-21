// Fill out your copyright notice in the Description page of Project Settings.


#include "CopaWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GeneralFunctionLibrary.h"
#include "FutebasSaveGame.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UCopaWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (!FutebasGI)
        return false;
    
    if (!ensure(jogar != nullptr))
        return false;
    jogar->OnClicked.AddDynamic(this, &UCopaWidget::jogarClicked);

    if (!ensure(salvar != nullptr))
        return false;
    salvar->OnClicked.AddDynamic(this, &UCopaWidget::salvarClicked);

    if (!ensure(sair != nullptr))
        return false;
    sair->OnClicked.AddDynamic(this, &UCopaWidget::sairClicked);

    proximo_jogo_player = false;

    teams_set = FutebasGI->current_teams_set;

    int32 teams_ind((int32)teams_set);
    if (FutebasGI->GetCopa(teams_ind)->fase_atual >= 1)
        gruposVisivel = ESlateVisibility::Hidden;
    else
        gruposVisivel = ESlateVisibility::Visible;
	tabelaVisivel = ESlateVisibility::Visible;

    // save automatico se acabou
    if (FutebasGI->current_teams_set == TeamsSet::Selecoes)
        if (FutebasGI->copa_do_mundo[teams_ind].fase_atual >= 4)
            if (FutebasGI->copa_do_mundo[teams_ind].faseFinal.fases[3].confrontos[0].getGanhador() >= 0) // copa ended
                UGeneralFunctionLibrary::saveGame(*FutebasGI->GetCopa(teams_ind), *FutebasGI->GetLiga(teams_ind), FutebasGI->team1, GameMode::CopaMundo, teams_set);

    return true;
}

void UCopaWidget::jogarClicked()
{
    if (FutebasGI)
    {
        if (!proximo_jogo_player)
        {
            proximo_jogo_player = FutebasGI->simulaJogosProximaRodada(GameMode::CopaMundo, teams_set);

            teams_set = FutebasGI->current_teams_set;

            int32 teams_ind((int32)teams_set);
            if (FutebasGI->GetCopa(teams_ind)->fase_atual >= 1)
            {
                gruposVisivel = ESlateVisibility::Hidden;
                tabelaVisivel = ESlateVisibility::Visible;
            }
        }
        else
        {
            proximo_jogo_player = false;
            FutebasGI->jogaPartida(GameMode::CopaMundo, teams_set);
        }
    }
}

void UCopaWidget::salvarClicked()
{
    int32 teams_ind((int32)teams_set);
    UGeneralFunctionLibrary::saveGame(*FutebasGI->GetCopa(teams_ind), *FutebasGI->GetLiga(teams_ind), FutebasGI->team1, GameMode::CopaMundo, teams_set);
}

void UCopaWidget::sairClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Menu_Level")));
}

ESlateVisibility UCopaWidget::bindCampeaoVisibilidade()
{
    int32 teams_ind((int32)teams_set);
    if (FutebasGI->GetCopa(teams_ind)->fase_atual > 4)
        if (FutebasGI->GetCopa(teams_ind)->faseFinal.fases[3].confrontos[0].getGanhador() != -1)
		    return ESlateVisibility::Visible;
    return ESlateVisibility::Hidden;
}

FSlateBrush UCopaWidget::bindCampeaoFlag()
{
    int32 teams_ind((int32)teams_set);
    if (FutebasGI)
        if (FutebasGI->GetCopa(teams_ind)->fase_atual > 4)
            if (FutebasGI->GetCopa(teams_ind)->faseFinal.fases[3].confrontos[0].getGanhador() != -1)
                return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(FutebasGI->GetCopa(teams_ind)->faseFinal.fases[3].confrontos[0].getGanhador(), GameMode::CopaMundo, teams_set).flag, 320, 240);
    return FSlateNoResource();
}