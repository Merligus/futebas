// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelSequenceActor.h"
#include "FutebasGameInstance.h"
#include "OJogoCharacter.h"
#include "OJogoGameState.h"
#include "FireworkActor.h"
#include "Sound/SoundWave.h"
#include "Sound/AmbientSound.h"
#include "Bola.h"
#include "BotCharacter.h"
#include "PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
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

	UPROPERTY(VisibleAnywhere)
	FTimerHandle beginGameTH;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle delayTimedOut;
	
	UPROPERTY(VisibleAnywhere)
	FVector posicao_bola;

	UPROPERTY(VisibleAnywhere)
	bool golTimeEsq;

	UPROPERTY(VisibleAnywhere)
	int32 faltamEsq_pen;

	UPROPERTY(VisibleAnywhere)
	bool golTimeDir;

	UPROPERTY(VisibleAnywhere)
	int32 faltamDir_pen;

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABotCharacter> botClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerCharacter> playerClass;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFutebasGameInstance* FutebasGI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AOJogoGameState* JogosGameState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AOJogoCharacter* botIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABola* BolaActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> PlayerStarts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* posicao;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool vezTimeEsquerdo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sons)
	AAmbientSound* comemorando_titulo;

public:
	AOJogoGameMode();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void possessRequested(APlayerController* PC, int32 team, FTeamData team_data);
	void possessRequested_Implementation(APlayerController* PC, int32 team, FTeamData team_data);

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
	bool golEsquerdo();

	UFUNCTION(BlueprintCallable)
	void golEsquerdoTimedOut();

	UFUNCTION(BlueprintCallable)
	bool golDireito();

	UFUNCTION(BlueprintCallable)
	void golDireitoTimedOut();

	UFUNCTION(BlueprintCallable)
	bool escanteio(AActor* pos);

	UFUNCTION(BlueprintCallable)
	void escanteioTimedOut();

	UFUNCTION(BlueprintCallable)
	void reiniciaBolaMeio();

	UFUNCTION(BlueprintCallable)
	void penalidadesMaximas();

	UFUNCTION(BlueprintCallable)
	void atualizaContagem();

	UFUNCTION(BlueprintCallable)
	TArray<int32> atualizaPenalidades(TArray<int32> array_pen, int32 pen);

	UFUNCTION(BlueprintCallable)
	void penaltyTimedOut();

	UFUNCTION(BlueprintCallable)
	void decideVencedor();

	UFUNCTION(BlueprintCallable)
	void comemoracaoTimedOut();

	UFUNCTION(BlueprintCallable)
	void setBotGols(int32 golsEsq, int32 golsDir);

	UFUNCTION(BlueprintCallable)
	void paralisaMovimentacao(bool ignora);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
