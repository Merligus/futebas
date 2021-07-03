// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FutebasGameInstance.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "TeamSlotPosicaoWidget.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UTeamSlotPosicaoWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UFutebasGameInstance* FutebasGI;

protected:

    virtual bool Initialize();
	
public:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* timeFlag;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* timeNome;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* timePontos;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* timePartidasJogadas;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* timeVitorias;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* timeEmpates;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* timeDerrotas;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* timeGolsPro;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* timeGolsContra;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* timeSaldoDeGols;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	int32 indexSlot;

	UFUNCTION(BlueprintCallable)
	FSlateBrush bindFlagTudo();
	
};
