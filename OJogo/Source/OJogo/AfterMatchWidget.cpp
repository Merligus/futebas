// Fill out your copyright notice in the Description page of Project Settings.


#include "AfterMatchWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "PlayerCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void UAfterMatchWidget::NativeConstruct()
{
    Super::NativeConstruct();

	JogosGameState = GetWorld()->GetAuthGameMode()->GetGameState<AOJogoGameState>();
	if (!JogosGameState)
    {
        GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("JogosGameState nao encontrado")));
		UE_LOG(LogTemp, Warning, TEXT("JogosGameState nao encontrado"));
    }
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

    APlayerCharacter* player1 = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    player1->SetUnpausable();

    return true;
}

void UAfterMatchWidget::continuarClicked()
{
    if (FutebasGI)
    {
        if (FutebasGI->copa_do_mundo.tabelaGrupos[0].times.Num() > 0)
        {
            FResultadoData resultado;
            if (JogosGameState)
            {
                resultado.index_casa = FutebasGI->team1.index_time;
                resultado.index_fora = FutebasGI->team2.index_time;
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
            FutebasGI->terminaPartida(resultado);
            UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Copa_Level")));
        }
        else
            UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Menu_Level")));
    }
    else
        GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("FutebasGI null after match")));
}

FSlateBrush UAfterMatchWidget::bindFlagTudo1()
{
	if (FutebasGI)
    {
        int32 index_casa = FutebasGI->team1.index_time;
        int32 gols_casa, gols_penalti_casa;
        if (JogosGameState)
        {
            gols_casa = JogosGameState->golsTimeEsq;
            gols_penalti_casa = JogosGameState->golsSomadosTimeEsq_pen;
        }
        else
        {
            gols_casa = 0;
            gols_penalti_casa = 0;
        }
        time1Nome->SetText(FText::FromString(FutebasGI->getTeamTrueIndex(index_casa).nome_hud));
        if (gols_casa >= 0)
            time1Gols->SetText(UKismetTextLibrary::Conv_IntToText(gols_casa));
        else
            time1Gols->SetText(FText::FromString(FString(TEXT(" "))));
        if (gols_penalti_casa >= 0)
            time1GolsPen->SetText(FText::FromString(FString(TEXT("(")) + FString::FromInt(gols_penalti_casa) + FString(TEXT(")")) ));
        else
            time1GolsPen->SetText(FText::FromString(FString(TEXT(""))));            
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeamTrueIndex(index_casa).flag, 64, 48);
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
	if (FutebasGI)
    {
        int32 index_fora = FutebasGI->team2.index_time;
        int32 gols_fora, gols_penalti_fora;
        if (JogosGameState)
        {
            gols_fora = JogosGameState->golsTimeDir;
            gols_penalti_fora = JogosGameState->golsSomadosTimeDir_pen;
        }
        else
        {
            gols_fora = 0;
            gols_penalti_fora = 0;
        }

        time2Nome->SetText(FText::FromString(FutebasGI->getTeamTrueIndex(index_fora).nome_hud));
        if (gols_fora >= 0)
            time2Gols->SetText(UKismetTextLibrary::Conv_IntToText(gols_fora));
        else
            time2Gols->SetText(FText::FromString(FString(TEXT(" "))));
        if (gols_penalti_fora >= 0)
            time2GolsPen->SetText(FText::FromString(FString(TEXT("(")) + FString::FromInt(gols_penalti_fora) + FString(TEXT(")")) ));
        else
            time2GolsPen->SetText(FText::FromString(FString(TEXT(""))));            
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeamTrueIndex(index_fora).flag, 64, 48);
    }
	else
    {
        time2Nome->SetText(FText::FromString(FString(TEXT("ERROR"))));
        time2Gols->SetText(FText::FromString(FString(TEXT(""))));
        time2GolsPen->SetText(FText::FromString(FString(TEXT(""))));
		return FSlateNoResource();
    }
}