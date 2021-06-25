// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FutebasGameInstance.h"
#include "OJogoCharacter.h"
#include "OJogoGameState.h"
#include "OJogoGameMode.generated.h"

/**
 * The GameMode defines the game being played. It governs the game rules, scoring, what actors
 * are allowed to exist in this game type, and who may enter the game.
 *
 * This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of OJogoCharacter
 */
UCLASS(minimalapi)
class AOJogoGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFutebasGameInstance* FutebasGI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AOJogoGameState* JogosGameState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AOJogoCharacter* player1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AOJogoCharacter* botIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AOJogoCharacter*> arrayJogadores;

public:
	AOJogoGameMode();

	UFUNCTION(BlueprintCallable)
	void beginGame();

	UFUNCTION(BlueprintCallable)
	void reiniciaPartida(bool neutro, bool favoravelEsq);

	UFUNCTION(BlueprintCallable)
	void antesDoComeco();

	UFUNCTION(BlueprintCallable)
	void setBotProprioGol(int32 golIndex);

	virtual void BeginPlay() override;
};
