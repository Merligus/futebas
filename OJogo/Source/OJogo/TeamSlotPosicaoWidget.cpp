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

    return true;
}

FSlateBrush UTeamSlotPosicaoWidget::bindFlagTudo()
{
	if (FutebasGI && FutebasGI->tabelaGrupos.Num() > 0)
    {
        FPosicaoData posicao;
        int32 times_por_grupo = FutebasGI->tabelaGrupos[0].n_times;
        int32 grupoAtual = indexSlot/times_por_grupo;
        int32 posicaoAtual = indexSlot % times_por_grupo;

        posicao = FutebasGI->tabelaGrupos[grupoAtual].tabela[posicaoAtual];

        timeNome->SetText(FText::FromString(FutebasGI->getTeam(posicao.index_time).nome_hud));
        timePontos->SetText(UKismetTextLibrary::Conv_IntToText(posicao.pontos));
        timePartidasJogadas->SetText(UKismetTextLibrary::Conv_IntToText(posicao.partidas_jogadas));
        timeVitorias->SetText(UKismetTextLibrary::Conv_IntToText(posicao.vitorias));
        timeEmpates->SetText(UKismetTextLibrary::Conv_IntToText(posicao.empates));
        timeDerrotas->SetText(UKismetTextLibrary::Conv_IntToText(posicao.derrotas));
        timeGolsPro->SetText(UKismetTextLibrary::Conv_IntToText(posicao.gols_pro));
        timeGolsContra->SetText(UKismetTextLibrary::Conv_IntToText(posicao.gols_contra));
        timeSaldoDeGols->SetText(UKismetTextLibrary::Conv_IntToText(posicao.saldo_de_gols));
		return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(posicao.index_time).flag, 32, 24);
    }
	else
    {
        timeNome->SetText(FText::FromString(FString(TEXT("TIME"))));
        timePontos->SetText(FText::FromString(FString(TEXT("0"))));
        timePartidasJogadas->SetText(FText::FromString(FString(TEXT("0"))));
        timeVitorias->SetText(FText::FromString(FString(TEXT("0"))));
        timeEmpates->SetText(FText::FromString(FString(TEXT("0"))));
        timeDerrotas->SetText(FText::FromString(FString(TEXT("0"))));
        timeGolsPro->SetText(FText::FromString(FString(TEXT("0"))));
        timeGolsContra->SetText(FText::FromString(FString(TEXT("0"))));
        timeSaldoDeGols->SetText(FText::FromString(FString(TEXT("0"))));
		return FSlateNoResource();
    }

}