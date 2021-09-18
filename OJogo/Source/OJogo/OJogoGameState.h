// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OJogoCharacter.h"
#include "Net/UnrealNetwork.h"
#include "OJogoGameState.generated.h"

//UENUM(BlueprintType)
//enum class CallFunction : uint8 {
//	penaltyTimedOut = 0 UMETA(DisplayName = "penaltyTimedOut"),
//	maisAcrescimos = 1 UMETA(DisplayName = "maisAcrescimos")
//};

class AOJogoGameMode;

/**
 * 
 */
UCLASS()
class OJOGO_API AOJogoGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sons)
	USoundWave* som_apito_inicio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sons)
	USoundWave* som_apito_fim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 golsTimeEsq;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 golsTimeDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bolaEmJogo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FTimespan acrescimos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 tempo1Ou2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool tempoRegulamentar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle tempo1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle tempo2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FVector posInicial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool vsBotIA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<int32> posIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<AOJogoCharacter*> arrayJogadores;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float tempoParadoAntesInicioPartida;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float tempoParado;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float tempoParadoEscanteio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float alturaReinicio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool em_prorrogacao;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool penalidades;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool timeDireitoPrimeiro_pen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool golEsquerdoAtivado_pen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool golDireitoAtivado_pen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 penalty_timeout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<int32> golsTimeEsq_pen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<int32> golsTimeDir_pen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 golsSomadosTimeEsq_pen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 golsSomadosTimeDir_pen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 playersSpawned;

	AOJogoGameState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void fazSomApito(int32 modo);
	void fazSomApito_Implementation(int32 modo);
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void paralisaMovimentacao(bool ignora);
	void paralisaMovimentacao_Implementation(bool ignora);
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void cutscene();
	void cutscene_Implementation();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void setTempo(bool tempo1b, float tempo, AOJogoGameMode* GameModePtr, int32 method);
	void setTempo_Implementation(bool tempo1b, float tempo, AOJogoGameMode* GameModePtr, int32 method);
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void openAfterMatch();
	void openAfterMatch_Implementation();
};
