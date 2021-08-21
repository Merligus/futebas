// Fill out your copyright notice in the Description page of Project Settings.


#include "LigaWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GeneralFunctionLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool ULigaWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (!FutebasGI)
        return false;
    
    if (!ensure(jogar != nullptr))
        return false;
    jogar->OnClicked.AddDynamic(this, &ULigaWidget::jogarClicked);

    if (!ensure(salvar != nullptr))
        return false;
    salvar->OnClicked.AddDynamic(this, &ULigaWidget::salvarClicked);

    if (!ensure(sair != nullptr))
        return false;
    tabelas->OnClicked.AddDynamic(this, &ULigaWidget::tabelaClicked);

    if (!ensure(sair != nullptr))
        return false;
    sair->OnClicked.AddDynamic(this, &ULigaWidget::sairClicked);

    proximo_jogo_player = false;
	tabelaVisivel = ESlateVisibility::Visible;
    partidasVisivel = ESlateVisibility::Hidden;

    teams_set = FutebasGI->current_teams_set;

    return true;
}

void ULigaWidget::jogarClicked()
{
    if (FutebasGI)
    {
        if (!proximo_jogo_player)
            proximo_jogo_player = FutebasGI->simulaJogosProximaRodada(GameMode::LigaNacoes, teams_set);
        else
        {
            proximo_jogo_player = false;
            FutebasGI->jogaPartida(GameMode::LigaNacoes, teams_set);
        }
    }
}

void ULigaWidget::salvarClicked()
{
    int32 teams_ind((int32)teams_set);
    UGeneralFunctionLibrary::saveGame(*FutebasGI->GetCopa(teams_ind), *FutebasGI->GetLiga(teams_ind), FutebasGI->team1, GameMode::LigaNacoes, teams_set);
}

void ULigaWidget::tabelaClicked()
{
    if (FutebasGI)
    {
        if (tabelaVisivel == ESlateVisibility::Visible)
        {
            tabelaVisivel = ESlateVisibility::Hidden;
            partidasVisivel = ESlateVisibility::Visible;
        }
        else
        {
            tabelaVisivel = ESlateVisibility::Visible;
            partidasVisivel = ESlateVisibility::Hidden;
        }
    }
}

void ULigaWidget::sairClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Menu_Level")));
}