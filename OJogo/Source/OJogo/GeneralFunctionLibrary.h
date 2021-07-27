// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FutebasGameInstance.h"
#include "TeamData.h"
#include "CopaMundoData.h"
#include "LigaData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeneralFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UGeneralFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void loadGame(const UObject* WorldContextObject, UFutebasGameInstance* FutebasGI, const GameMode game_mode, const TeamsSet teams_set);

	UFUNCTION(BlueprintCallable)
	static void saveGame(const FCopaMundoData CdM, const FLigaData LdN, const FTeamData team1, const GameMode game_mode, const TeamsSet teams_set);
	
};
