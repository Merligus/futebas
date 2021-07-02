// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FutebasGameInstance.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "TeamSlotWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTeamSlotDelegate, int32, index);

UCLASS()
class OJOGO_API UTeamSlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UFutebasGameInstance* FutebasGI;

protected:

    virtual bool Initialize();
	
public:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* timeFlag;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* timeBotao;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* timeNome;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	int32 indexSlot;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FTeamSlotDelegate indexPassDelegate;

	void ButtonWasClicked(int32 index);

	UFUNCTION(BlueprintCallable)
	void ButtonClicked();

	UFUNCTION(BlueprintCallable)
	FSlateBrush bindFlagNome();
};
