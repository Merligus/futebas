// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

    virtual bool Initialize();
	
public:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* Continuar;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* opcoes;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* sair;

	UFUNCTION(BlueprintCallable)
	void ContinuarClicked();

	UFUNCTION(BlueprintCallable)
	void opcoesClicked();

	UFUNCTION(BlueprintCallable)
	void sairClicked();	
};
