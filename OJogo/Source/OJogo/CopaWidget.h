// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FutebasGameInstance.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "CopaWidget.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UCopaWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UFutebasGameInstance* FutebasGI;

protected:

    virtual bool Initialize();
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TeamsSet teams_set;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* jogar;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* salvar;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* sair;

	UPROPERTY(BlueprintReadWrite)
	bool proximo_jogo_player;

	UPROPERTY(BlueprintReadWrite)
	ESlateVisibility tabelaVisivel;

	UPROPERTY(BlueprintReadWrite)
	ESlateVisibility gruposVisivel;

	UFUNCTION(BlueprintCallable)
	void jogarClicked();

	UFUNCTION(BlueprintCallable)
	void salvarClicked();

	UFUNCTION(BlueprintCallable)
	void sairClicked();

	UFUNCTION(BlueprintCallable)
	ESlateVisibility bindCampeaoVisibilidade();

	UFUNCTION(BlueprintCallable)
	FSlateBrush bindCampeaoFlag();
};
