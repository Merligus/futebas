// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Kismet/GameplayStatics.h"

bool UPauseWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

    if (!ensure(Continuar != nullptr))
        return false;
    Continuar->OnClicked.AddDynamic(this, &UPauseWidget::ContinuarClicked);

    if (!ensure(sair != nullptr))
        return false;
    sair->OnClicked.AddDynamic(this, &UPauseWidget::sairClicked);

    return true;
}

void UPauseWidget::ContinuarClicked()
{
    RemoveFromParent();
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    PC->SetShowMouseCursor(false);
}

void UPauseWidget::sairClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Menu_Level")));
}