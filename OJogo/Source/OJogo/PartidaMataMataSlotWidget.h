// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FutebasGameInstance.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "PartidaMataMataSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UPartidaMataMataSlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UFutebasGameInstance* FutebasGI;

protected:

    virtual bool Initialize();
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TeamsSet teams_set;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	int32 fase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	int32 index_jogo;

	UFUNCTION(BlueprintCallable)
	FSlateBrush bindFlagTudo1();

	UFUNCTION(BlueprintCallable)
	FSlateBrush bindFlagTudo2();
	
};
