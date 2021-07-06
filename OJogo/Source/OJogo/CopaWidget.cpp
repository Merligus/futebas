// Fill out your copyright notice in the Description page of Project Settings.


#include "CopaWidget.h"
#include "Kismet/KismetSystemLibrary.h"
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

    if (!ensure(sair != nullptr))
        return false;
    sair->OnClicked.AddDynamic(this, &UCopaWidget::sairClicked);

    return true;
}

void UCopaWidget::jogarClicked()
{
    if (FutebasGI)
        FutebasGI->simulaJogosProximaRodada();
}

void UCopaWidget::sairClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Type::Background, false);
}

ESlateVisibility UCopaWidget::bindCampeaoVisibilidade()
{
    if (FutebasGI->copa_do_mundo.fase_atual > 4)
        if (FutebasGI->copa_do_mundo.faseFinal.fases[3].confrontos[0].getGanhador() != -1)
		    return ESlateVisibility::Visible;
    return ESlateVisibility::Hidden;
}

FSlateBrush UCopaWidget::bindCampeaoFlag()
{
    if (FutebasGI)
        if (FutebasGI->copa_do_mundo.fase_atual > 4)
            if (FutebasGI->copa_do_mundo.faseFinal.fases[3].confrontos[0].getGanhador() != -1)
                return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(FutebasGI->copa_do_mundo.faseFinal.fases[3].confrontos[0].getGanhador()).flag, 320, 240);
    return FSlateNoResource();
}