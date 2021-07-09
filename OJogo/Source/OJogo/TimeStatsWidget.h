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
	void sorteiaGruposCopa();

	UFUNCTION(BlueprintCallable)
	bool bindStatsEnabled(bool team1 = true);

	UFUNCTION(BlueprintCallable)
	FText bindTeamName(bool team1 = true);

	UFUNCTION(BlueprintCallable)
	float bindChute(bool team1 = true);

	UFUNCTION(BlueprintCallable)
	float bindVelocidade(bool team1 = true);

	UFUNCTION(BlueprintCallable)
	float bindCarrinho(bool team1 = true);

	UFUNCTION(BlueprintCallable)
	float bindEnergia(bool team1 = true);

	UFUNCTION(BlueprintCallable)
	void bindIndexTimeGrupos();
};
