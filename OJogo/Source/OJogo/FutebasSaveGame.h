// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FutebasGameInstance.h"
#include "TeamData.h"
#include "CopaMundoData.h"
#include "LigaData.h"
#include "GameFramework/SaveGame.h"
#include "FutebasSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UFutebasSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCopaMundoData copa_do_mundo;	// campeonato state

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLigaData liga_das_nacoes;	// campeonato state

	UFutebasSaveGame();
	
};
