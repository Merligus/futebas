// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FutebasGameInstance.h"
#include "OJogoGameState.h"
#include "ResultadoData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "AfterMatchWidget.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UAfterMatchWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UFutebasGameInstance* FutebasGI;

	UPROPERTY()
	AOJogoGameState* JogosGameState;

protected:

    virtual bool Initialize();

	virtual void NativeConstruct() override;
	
public:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* continuar;

	UFUNCTION(BlueprintCallable)
	void continuarClicked();

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* time1Flag;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* time2Flag;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* time1Nome;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* time2Nome;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* time1Gols;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* time2Gols;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* time1GolsPen;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* time2GolsPen;

	UFUNCTION(BlueprintCallable)
	FSlateBrush bindFlagTudo1();

	UFUNCTION(BlueprintCallable)
	FSlateBrush bindFlagTudo2();
};
