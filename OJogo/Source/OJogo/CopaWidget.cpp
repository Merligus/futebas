// Fill out your copyright notice in the Description page of Project Settings.


#include "CopaWidget.h"
#include "Kismet/KismetSystemLibrary.h"

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