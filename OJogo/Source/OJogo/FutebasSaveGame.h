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
	TArray<FCopaMundoData> copa_do_mundo;	// campeonato state

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLigaData> liga_das_nacoes;	// campeonato state

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> volumes;	// settings state

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CDM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LDN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 gols_unica_copa;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 copas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 gols_todas_copas;

	UFutebasSaveGame();

	FCopaMundoData* GetCopa(int32 index);

	FLigaData* GetLiga(int32 index);

	void SetCopa(FCopaMundoData copa, int32 index);

	void SetLiga(FLigaData liga, int32 index);
	
};
