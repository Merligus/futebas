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

    if (!ensure(timeBotao != nullptr))
        return false;
    timeBotao->OnClicked.AddDynamic(this, &UTeamSlotWidget::ButtonClicked);

    return true;
}

void UTeamSlotWidget::ButtonClicked()
{
    if (FutebasGI)
        FutebasGI->team1 = FutebasGI->getTeam(indexSlot);
    else
        GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Slot %d clicado mas FutebasGI null"), indexSlot));
}

FSlateBrush UTeamSlotWidget::bindFlagNome()
{
	if (FutebasGI)
    {
        timeNome->SetText(FText::FromString(FutebasGI->getTeam(indexSlot).nome_hud));
		return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(indexSlot).flag, 36, 24);
    }
	else
    {
        timeNome->SetText(FText::FromString(FString(TEXT("ERROR"))));
		return FSlateNoResource();
    }
}