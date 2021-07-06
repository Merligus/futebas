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

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* jogar;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* sair;

	UFUNCTION(BlueprintCallable)
	void jogarClicked();

	UFUNCTION(BlueprintCallable)
	void sairClicked();

	UFUNCTION(BlueprintCallable)
	ESlateVisibility bindCampeaoVisibilidade();

	UFUNCTION(BlueprintCallable)
	FSlateBrush bindCampeaoFlag();
};
