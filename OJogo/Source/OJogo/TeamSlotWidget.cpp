// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamSlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UTeamSlotWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

	FTeamData b;

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (!FutebasGI)
		b = FutebasGI->getTeam(0);
	else
		UE_LOG(LogTemp, Warning, TEXT("Futebas nao encontrado"));

    if (!ensure(timeBotao != nullptr))
        return false;
    timeBotao->OnClicked.AddDynamic(this, &UTeamSlotWidget::ButtonClicked);

    indexPassDelegate.AddDynamic(this, &UTeamSlotWidget::ButtonWasClicked);

    return true;
}

void UTeamSlotWidget::ButtonWasClicked(int32 index)
{

}

void UTeamSlotWidget::ButtonClicked()
{
    indexPassDelegate.Broadcast(indexSlot);
}

FSlateBrush UTeamSlotWidget::bindFlagNome()
{
	if (FutebasGI)
    {
        timeNome->SetText(FText::FromString(FutebasGI->getTeam(indexSlot).nome_hud));
		return UWidgetBlueprintLibrary::MakeBrushFromTexture(FutebasGI->getTeam(indexSlot).flag, 640, 480);
    }
	else
    {
        timeNome->SetText(FText::FromString(FString(TEXT("ERROR"))));
		return FSlateNoResource();
    }
}