// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OJogoCharacter.h"
#include "OJogoGameState.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API AOJogoGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* Bola_c;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TSubclassOf<AOJogoCharacter> player1_c;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* botIA_c;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 golsTimeEsq;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 golsTimeDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bolaEmJogo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimespan acrescimos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 tempo1Ou2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool tempoRegulamentar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle tempo1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle tempo2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector posInicial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool vsBotIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> posIndex;

	AOJogoGameState();
	
};
