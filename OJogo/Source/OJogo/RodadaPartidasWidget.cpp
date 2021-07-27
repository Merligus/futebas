// Fill out your copyright notice in the Description page of Project Settings.


#include "RodadaPartidasWidget.h"

void URodadaPartidasWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

bool URodadaPartidasWidget::Initialize()
{
    int32 teams_ind((int32)teams_set);
    bool success = Super::Initialize();
    if (!success)
        return false;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (!FutebasGI)
        return false;

    if (IsValid(rodadaSlider))
    {
        rodadaSlider->OnValueChanged.AddDynamic(this, &URodadaPartidasWidget::SliderValue);
        rodadaSlider->SetMinValue(1);
        if (game_mode == GameMode::CopaMundo)
            rodadaSlider->SetMaxValue(FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].times.Num()-1);
        else if (game_mode == GameMode::LigaNacoes)
            rodadaSlider->SetMaxValue(2*(FutebasGI->GetLiga(teams_ind)->tabelas[grupo].times.Num()-1));
    }
    else
        return false;

    int32 max_rodadas(1), rodada(0), n_jogos(2);
    if (game_mode == GameMode::CopaMundo)
    {
        max_rodadas = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].times.Num()-1;
        rodada = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].rodada_atual;
        n_jogos = FutebasGI->GetCopa(teams_ind)->tabelaGrupos[grupo].calendario[FMath::Clamp(rodada, 0, max_rodadas-1)].jogos.Num();
    }
    else if (game_mode == GameMode::LigaNacoes)
    {
        max_rodadas = FutebasGI->GetLiga(teams_ind)->tabelas[grupo].times.Num()-1;
        rodada = FutebasGI->GetLiga(teams_ind)->tabelas[grupo].rodada_atual;
        n_jogos = FutebasGI->GetLiga(teams_ind)->tabelas[grupo].calendario[FMath::Clamp(rodada, 0, max_rodadas-1)].jogos.Num();
    }
    
    rodadaSlider->SetValue(rodada+1);
    for (int32 index_jogo = 0; index_jogo < n_jogos; ++index_jogo)
    {
        if (IsValid(widgetClass))
        {
            UWidget* widgetContent = CreateWidget(GetWorld(), widgetClass);
            partidaSlotWidget = Cast<UPartidaSlotWidget>(widgetContent);
            partidaSlotWidget->game_mode = game_mode;
            partidaSlotWidget->index_jogo = index_jogo;
            partidaSlotWidget->grupo = grupo;
            partidaSlotWidget->rodada = rodada+1;
            partidasSlots.Add(partidaSlotWidget);
            PartidasBox->AddChildToVerticalBox(widgetContent);
        }
    }
    return true;
}

void URodadaPartidasWidget::SliderValue(float InValue)
{
    for (int32 index = 0; index < partidasSlots.Num(); ++index)
        partidasSlots[index]->rodada = FMath::TruncToInt(InValue);
}