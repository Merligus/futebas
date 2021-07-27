// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamSlotPosicaoWidget.h"
#include "Kismet/KismetTextLibrary.h"
#include "PosicaoData.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UTeamSlotPosicaoWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (!FutebasGI)
        return false;

    teams_set = FutebasGI->current_teams_set;

    return true;
}

FSlateBrush UTeamSlotPosicaoWidget::bindFlagTudo()
{
    int32 teams_ind((int32)teams_set);
    if (FutebasGI)
    {
        if (game_mode == GameMode::CopaMundo)
        {
            if (indexSlot >= 0)
            {
                FPosicaoData posicao;
                int32 times_por_grupo = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[0].n_times;
                int32 grupoAtual = indexSlot/times_por_grupo;
                int32 posicaoAtual = indexSlot % times_por_grupo;

                posicao = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupoAtual].tabela[posicaoAtual];

                timePos->SetText(UKismetTextLibrary::Conv_IntToText(posicaoAtual+1));
                timeNome->SetText(FText::FromString(FutebasGI->getTeam(posicao.index_time, game_mode, teams_set).nome_hud));
                timePontos->SetText(UKismetTextLibrary::Conv_IntToText(posicao.pontos));
                timePartidasJogadas->SetText(UKismetTextLibrary::Conv_IntToText(posicao.partidas_jogadas));
                timeVitorias->SetText(UKismetTextLibrary::Conv_IntToText(posicao.vitorias));
                timeEmpates->SetText(UKismetTextLibrary::Conv_IntToText(posicao.empates));
                timeDerrotas->SetText(UKismetTextLibrary::Conv_IntToText(posicao.derrotas));
                timeGolsPro->SetText(UKismetTextLibrary::Conv_IntToText(posicao.gols_pro));
                timeGolsContra->SetText(UKismetTextLibrary::Conv_IntToText(posicao.gols_contra));
                timeSaldoDeGols->SetText(UKismetTextLibrary::Conv_IntToText(posicao.saldo_de_gols));
                return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(posicao.index_time, game_mode, teams_set).flag, 32, 24);
            }
            else
            {
                timePos->SetText(FText());
                timeNome->SetText(FText::FromString(FString(TEXT("Equipe"))));
                timePontos->SetText(FText::FromString(FString(TEXT("P"))));
                timePartidasJogadas->SetText(FText::FromString(FString(TEXT("PJ"))));
                timeVitorias->SetText(FText::FromString(FString(TEXT("V"))));
                timeEmpates->SetText(FText::FromString(FString(TEXT("E"))));
                timeDerrotas->SetText(FText::FromString(FString(TEXT("D"))));
                timeGolsPro->SetText(FText::FromString(FString(TEXT("GP"))));
                timeGolsContra->SetText(FText::FromString(FString(TEXT("GC"))));
                timeSaldoDeGols->SetText(FText::FromString(FString(TEXT("SG"))));
                FSlateBrush inv_img = FSlateNoResource();
                inv_img.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
                return inv_img;
            }
        }
        else if (game_mode == GameMode::LigaNacoes)
        {
            if (indexSlot >= 0)
            {
                FPosicaoData posicao;
                int32 times_por_grupo = FutebasGI->GetLiga(teams_ind)->tabelas[0].n_times;
                int32 grupoAtual = indexSlot/times_por_grupo;
                int32 posicaoAtual = indexSlot % times_por_grupo;

                posicao = FutebasGI->GetLiga(teams_ind)->tabelas[grupoAtual].tabela[posicaoAtual];

                timePos->SetText(UKismetTextLibrary::Conv_IntToText(posicaoAtual+1));
                timeNome->SetText(FText::FromString(FutebasGI->getTeam(posicao.index_time, game_mode, teams_set).nome_hud));
                timePontos->SetText(UKismetTextLibrary::Conv_IntToText(posicao.pontos));
                timePartidasJogadas->SetText(UKismetTextLibrary::Conv_IntToText(posicao.partidas_jogadas));
                timeVitorias->SetText(UKismetTextLibrary::Conv_IntToText(posicao.vitorias));
                timeEmpates->SetText(UKismetTextLibrary::Conv_IntToText(posicao.empates));
                timeDerrotas->SetText(UKismetTextLibrary::Conv_IntToText(posicao.derrotas));
                timeGolsPro->SetText(UKismetTextLibrary::Conv_IntToText(posicao.gols_pro));
                timeGolsContra->SetText(UKismetTextLibrary::Conv_IntToText(posicao.gols_contra));
                timeSaldoDeGols->SetText(UKismetTextLibrary::Conv_IntToText(posicao.saldo_de_gols));
                return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(posicao.index_time, game_mode, teams_set).flag, 32, 24);
            }
            else
            {
                timePos->SetText(FText());
                timeNome->SetText(FText::FromString(FString(TEXT("Equipe"))));
                timePontos->SetText(FText::FromString(FString(TEXT("P"))));
                timePartidasJogadas->SetText(FText::FromString(FString(TEXT("PJ"))));
                timeVitorias->SetText(FText::FromString(FString(TEXT("V"))));
                timeEmpates->SetText(FText::FromString(FString(TEXT("E"))));
                timeDerrotas->SetText(FText::FromString(FString(TEXT("D"))));
                timeGolsPro->SetText(FText::FromString(FString(TEXT("GP"))));
                timeGolsContra->SetText(FText::FromString(FString(TEXT("GC"))));
                timeSaldoDeGols->SetText(FText::FromString(FString(TEXT("SG"))));
                FSlateBrush inv_img = FSlateNoResource();
                inv_img.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
                return inv_img;
            }
        }       
        else
        {
            timePos->SetText(FText());
            timeNome->SetText(FText::FromString(FString(TEXT("Equipe"))));
            timePontos->SetText(FText::FromString(FString(TEXT("P"))));
            timePartidasJogadas->SetText(FText::FromString(FString(TEXT("PJ"))));
            timeVitorias->SetText(FText::FromString(FString(TEXT("V"))));
            timeEmpates->SetText(FText::FromString(FString(TEXT("E"))));
            timeDerrotas->SetText(FText::FromString(FString(TEXT("D"))));
            timeGolsPro->SetText(FText::FromString(FString(TEXT("GP"))));
            timeGolsContra->SetText(FText::FromString(FString(TEXT("GC"))));
            timeSaldoDeGols->SetText(FText::FromString(FString(TEXT("SG"))));
            FSlateBrush inv_img = FSlateNoResource();
            inv_img.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
            return inv_img;
        }
    }
	else
    {
        timePos->SetText(FText());
        timeNome->SetText(FText::FromString(FString(TEXT("Equipe"))));
        timePontos->SetText(FText::FromString(FString(TEXT("P"))));
        timePartidasJogadas->SetText(FText::FromString(FString(TEXT("PJ"))));
        timeVitorias->SetText(FText::FromString(FString(TEXT("V"))));
        timeEmpates->SetText(FText::FromString(FString(TEXT("E"))));
        timeDerrotas->SetText(FText::FromString(FString(TEXT("D"))));
        timeGolsPro->SetText(FText::FromString(FString(TEXT("GP"))));
        timeGolsContra->SetText(FText::FromString(FString(TEXT("GC"))));
        timeSaldoDeGols->SetText(FText::FromString(FString(TEXT("SG"))));
        FSlateBrush inv_img = FSlateNoResource();
        inv_img.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
		return inv_img;
    }

}