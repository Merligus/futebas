// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FutebasGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "TimeStatsWidget.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UTimeStatsWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UFutebasGameInstance* FutebasGI;

public:

	virtual bool Initialize();

	UFUNCTION(BlueprintCallable)
	bool bindStatsEnabled();

	UFUNCTION(BlueprintCallable)
	FText bindTeamName();

	UFUNCTION(BlueprintCallable)
	float bindChute();

	UFUNCTION(BlueprintCallable)
	float bindVelocidade();

	UFUNCTION(BlueprintCallable)
	float bindCarrinho();

	UFUNCTION(BlueprintCallable)
	float bindEnergia();
};
