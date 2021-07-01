// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FutebasGameInstance.h"
#include "OJogoGameState.h"
#include "Components/Button.h"
#include "Components/Image.h"
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

	UPROPERTY()
	AOJogoGameState* JogosGameState;

protected:

    virtual bool Initialize();

public:

	// UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	// UButton* Button_535;

	// UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	// UImage* timeA1Flag;

	// UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	// UTextBlock* timeA1Nome;
	
    // void ButtonClicked();
};
