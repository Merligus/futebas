// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FutebasGameInstance.h"
#include "OJogoCharacter.h"
#include "OJogoGameState.h"
#include "Components/Image.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UFutebasGameInstance* FutebasGI;

	UPROPERTY()
	AOJogoGameState* JogosGameState;

	UPROPERTY()
	bool penaltis;

public:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* image_esq1;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* image_esq2;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* image_esq3;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* image_esq4;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* image_esq5;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* image_dir1;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* image_dir2;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* image_dir3;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* image_dir4;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* image_dir5;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	FText bindGoalsEsq();

	UFUNCTION(BlueprintCallable)
	FText bindGoalsDir();

	UFUNCTION(BlueprintCallable)
	FText bindMinutes();

	UFUNCTION(BlueprintCallable)
	FText bindSeconds();

	UFUNCTION(BlueprintCallable)
	FText bindAcrescimosMinutos();

	UFUNCTION(BlueprintCallable)
	ESlateVisibility bindTempoNaoRegulamentarVisibilidade();

	UFUNCTION(BlueprintCallable)
	FSlateBrush bindTime1Emb();

	UFUNCTION(BlueprintCallable)
	FSlateBrush bindTime2Emb();

	UFUNCTION(BlueprintCallable)
	FText bindTime1Nome();

	UFUNCTION(BlueprintCallable)
	FText bindTime2Nome();
	
	UFUNCTION(BlueprintCallable)
	FLinearColor bindTime1Cor1();

	UFUNCTION(BlueprintCallable)
	FLinearColor bindTime1Cor2();

	UFUNCTION(BlueprintCallable)
	FLinearColor bindTime2Cor1();

	UFUNCTION(BlueprintCallable)
	FLinearColor bindTime2Cor2();

	UFUNCTION(BlueprintCallable)
	ESlateVisibility tempoPenalidadesVisibilidade();

	UFUNCTION(BlueprintCallable)
	ESlateVisibility tempoNormalVisibilidade();

	UFUNCTION(BlueprintCallable)
	FText bindGoalsEsqPen(FSlateBrush erro, FSlateBrush acerto, FSlateBrush indefinido);

	UFUNCTION(BlueprintCallable)
	FText bindGoalsDirPen(FSlateBrush erro, FSlateBrush acerto, FSlateBrush indefinido);

	UFUNCTION(BlueprintCallable)
	FText bindMinutesPenalidades();

	UFUNCTION(BlueprintCallable)
	FText bindSecondsPenalidades();
};
