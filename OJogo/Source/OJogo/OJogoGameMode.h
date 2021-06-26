// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FutebasGameInstance.h"
#include "OJogoCharacter.h"
#include "OJogoGameState.h"
#include "Bola.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* posicao;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool vezTimeEsquerdo;

public:
	AOJogoGameMode();

	UFUNCTION(BlueprintCallable)
	void beginGame();

	UFUNCTION(BlueprintCallable)
	void reiniciaPartida(bool neutro, bool favoravelEsq);

	UFUNCTION(BlueprintCallable)
	void antesDoComeco();

	UFUNCTION(BlueprintCallable)
	void antesDoComecoTimedOut();

	UFUNCTION(BlueprintCallable)
	void comecaJogo();

	UFUNCTION(BlueprintCallable)
	void maisAcrescimos();

	UFUNCTION(BlueprintCallable)
	void terminaTempo();
	
	UFUNCTION(BlueprintCallable)
	void terminaTempoTimedOut();

	UFUNCTION(BlueprintCallable)
	void fimDePapo();

	UFUNCTION(BlueprintCallable)
	void trocaTimes();

	UFUNCTION(BlueprintCallable)
	void golEsquerdo();

	UFUNCTION(BlueprintCallable)
	void golEsquerdoTimedOut();

	UFUNCTION(BlueprintCallable)
	void golDireito();

	UFUNCTION(BlueprintCallable)
	void golDireitoTimedOut();

	UFUNCTION(BlueprintCallable)
	void escanteio(AActor* pos);

	UFUNCTION(BlueprintCallable)
	void escanteioTimedOut();

	UFUNCTION(BlueprintCallable)
	void reiniciaBolaMeio();

	UFUNCTION(BlueprintCallable)
	void penalidadesMaximas();

	UFUNCTION(BlueprintCallable)
	void atualizaContagem();

	UFUNCTION(BlueprintCallable)
	void decideVencedor(FString timeVencedor);

	UFUNCTION(BlueprintCallable)
	void setBotProprioGol(int32 golIndex);

	UFUNCTION(BlueprintCallable)
	void setBotGols(int32 golsEsq, int32 golsDir);

	UFUNCTION(BlueprintCallable)
	void paralisaMovimentacao(bool ignora);

	virtual void BeginPlay() override;
};
