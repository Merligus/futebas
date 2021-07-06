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
	if (FutebasGI && FutebasGI->copa_do_mundo.tabelaGrupos.Num() > 0 && indexSlot >= 0)
    {
        FPosicaoData posicao;
        int32 times_por_grupo = FutebasGI->copa_do_mundo.tabelaGrupos[0].n_times;
        int32 grupoAtual = indexSlot/times_por_grupo;
        int32 posicaoAtual = indexSlot % times_por_grupo;

        posicao = FutebasGI->copa_do_mundo.tabelaGrupos[grupoAtual].tabela[posicaoAtual];

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