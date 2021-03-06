// Fill out your copyright notice in the Description page of Project Settings.


#include "AfterMatchWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "PlayerCharacter.h"
#include "FutebasSaveGame.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerStart.h"

void UAfterMatchWidget::NativeConstruct()
{
    Super::NativeConstruct();

	// JogosGameState = GetWorld()->GetAuthGameMode()->GetGameState<AOJogoGameState>();
    JogosGameState = GetWorld() != NULL ? GetWorld()->GetGameState<AOJogoGameState>() : NULL;
	if (!JogosGameState)
		UE_LOG(LogTemp, Warning, TEXT("JogosGameState nao encontrado"));
}

bool UAfterMatchWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (!FutebasGI)
        return false;

    if (!ensure(continuar != nullptr))
        return false;
    continuar->OnClicked.AddDynamic(this, &UAfterMatchWidget::continuarClicked);

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	for (int32 Index = 0; Index < FoundActors.Num(); ++Index)
    {
        APlayerCharacter* player1 = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), Index));
        if (IsValid(player1))
            player1->SetUnpausable();
    }

    return true;
}

void UAfterMatchWidget::continuarClicked()
{
    if (FutebasGI)
    {
        if (FutebasGI->current_game_mode == GameMode::CopaMundo)
        {
            FResultadoData resultado;
            if (JogosGameState)
            {
                resultado = FResultadoData(JogosGameState->golsTimeEsq, JogosGameState->golsSomadosTimeEsq_pen, 
                                           JogosGameState->golsTimeDir, JogosGameState->golsSomadosTimeDir_pen);
                if (!FutebasGI->team1_em_casa)
                {
                    FTeamData auxTeam;
                    auxTeam = FutebasGI->team1;
                    FutebasGI->team1 = FutebasGI->team2;
                    FutebasGI->team2 = auxTeam;
                }
            }
            else
                resultado = FResultadoData(3);
            resultado.index_casa = FutebasGI->team1.index_time;
            resultado.index_fora = FutebasGI->team2.index_time;
            FutebasGI->terminaPartida(resultado, GameMode::CopaMundo, FutebasGI->current_teams_set);
            
            // save copas e gols totais se acabou
            if (FutebasGI->current_teams_set == TeamsSet::Selecoes)
            {
                int32 teams_ind((int32)FutebasGI->current_teams_set);
                UFutebasSaveGame* FutebasSG;
                if (UGameplayStatics::DoesSaveGameExist(FString(TEXT("FutebasSavedGame")), 0))
                    FutebasSG = Cast<UFutebasSaveGame>(UGameplayStatics::LoadGameFromSlot(FString(TEXT("FutebasSavedGame")), 0));
                else
                    FutebasSG = Cast<UFutebasSaveGame>(UGameplayStatics::CreateSaveGameObject(UFutebasSaveGame::StaticClass()));

                bool campeao_b(FutebasGI->copa_do_mundo[teams_ind].isCampeao(FutebasGI->team1.index_time));
                FutebasSG->copas += campeao_b? 1 : 0;
                if (FutebasGI->copa_do_mundo[teams_ind].fase_atual >= 4)
                    if (FutebasGI->copa_do_mundo[teams_ind].faseFinal.fases[3].confrontos[0].getGanhador() >= 0) // copa ended
                        FutebasSG->gols_todas_copas += FutebasGI->copa_do_mundo[teams_ind].getGolsTime(FutebasGI->team1.index_time);
                    
                UGameplayStatics::SaveGameToSlot(FutebasSG, FString(TEXT("FutebasSavedGame")), 0);
            }
            UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Copa_Level")));
        }
        else if (FutebasGI->current_game_mode == GameMode::LigaNacoes)
        {
            FResultadoData resultado;
            if (JogosGameState)
            {
                resultado = FResultadoData(JogosGameState->golsTimeEsq, JogosGameState->golsSomadosTimeEsq_pen, 
                                           JogosGameState->golsTimeDir, JogosGameState->golsSomadosTimeDir_pen);
                if (!FutebasGI->team1_em_casa)
                {
                    FTeamData auxTeam;
                    auxTeam = FutebasGI->team1;
                    FutebasGI->team1 = FutebasGI->team2;
                    FutebasGI->team2 = auxTeam;
                }
            }
            else
                resultado = FResultadoData(3);
            resultado.index_casa = FutebasGI->team1.index_time;
            resultado.index_fora = FutebasGI->team2.index_time;
            FutebasGI->terminaPartida(resultado, GameMode::LigaNacoes, FutebasGI->current_teams_set);
            UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Liga_Nacoes_Level")));
        }
        else
            UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Menu_Level")));
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("FutebasGI null after match"));
}

FSlateBrush UAfterMatchWidget::bindFlagTudo1()
{
	if (JogosGameState)
    {
        int32 index_casa = JogosGameState->team1.index_time;
        int32 gols_casa, gols_penalti_casa;
        gols_casa = JogosGameState->golsTimeEsq;
        gols_penalti_casa = JogosGameState->golsSomadosTimeEsq_pen;

        time1Nome->SetText(FText::FromString(JogosGameState->team1.nome_hud));
        if (gols_casa >= 0)
            time1Gols->SetText(UKismetTextLibrary::Conv_IntToText(gols_casa));
        else
            time1Gols->SetText(FText::FromString(FString(TEXT(" "))));
        if (gols_penalti_casa >= 0)
            time1GolsPen->SetText(FText::FromString(FString(TEXT("(")) + FString::FromInt(gols_penalti_casa) + FString(TEXT(")"))));
        else
            time1GolsPen->SetText(FText::FromString(FString(TEXT(""))));
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(JogosGameState->team1.flag, 64, 48);
    }
    else
    {
        time1Nome->SetText(FText::FromString(FString(TEXT("ERROR"))));
        time1Gols->SetText(FText::FromString(FString(TEXT(""))));
        time1GolsPen->SetText(FText::FromString(FString(TEXT(""))));
        return FSlateNoResource();
    }
}

FSlateBrush UAfterMatchWidget::bindFlagTudo2()
{
    if (JogosGameState)
    {
        int32 index_fora = JogosGameState->team2.index_time;
        int32 gols_fora, gols_penalti_fora;
        gols_fora = JogosGameState->golsTimeDir;
        gols_penalti_fora = JogosGameState->golsSomadosTimeDir_pen;

        time2Nome->SetText(FText::FromString(JogosGameState->team2.nome_hud));
        if (gols_fora >= 0)
            time2Gols->SetText(UKismetTextLibrary::Conv_IntToText(gols_fora));
        else
            time2Gols->SetText(FText::FromString(FString(TEXT(" "))));
        if (gols_penalti_fora >= 0)
            time2GolsPen->SetText(FText::FromString(FString(TEXT("(")) + FString::FromInt(gols_penalti_fora) + FString(TEXT(")"))));
        else
            time2GolsPen->SetText(FText::FromString(FString(TEXT(""))));
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(JogosGameState->team2.flag, 64, 48);
    }
	else
    {
        time2Nome->SetText(FText::FromString(FString(TEXT("ERROR"))));
        time2Gols->SetText(FText::FromString(FString(TEXT(""))));
        time2GolsPen->SetText(FText::FromString(FString(TEXT(""))));
		return FSlateNoResource();
    }
}