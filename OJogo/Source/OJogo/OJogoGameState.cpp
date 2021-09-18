// Fill out your copyright notice in the Description page of Project Settings.


#include "OJogoGameState.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "AfterMatchWidget.h"
#include "OJogoGameMode.h"
#include "Camera/CameraActor.h"

AOJogoGameState::AOJogoGameState()
{
	// botIA_c = LoadClass<AOJogoCharacter>(NULL, TEXT("/Game/Teams/IA/Bot.Bot_C"), NULL, 0, NULL);

    golsTimeEsq = 0;
	golsTimeDir = 0;
	bolaEmJogo = false;
	acrescimos = FTimespan(0, 0, 0);
	tempo1Ou2 = 1;
	tempoRegulamentar = true;
	posInicial = FVector(0.0f);
	posIndex.Add(0);
	posIndex.Add(1);
	tempoParadoAntesInicioPartida = 3.0f;
	tempoParado = 2.0f;
	tempoParadoEscanteio = 1.0f;
	alturaReinicio = 800.0f;
	penalty_timeout = 4;

	em_prorrogacao = false;
	penalidades = false;
	timeDireitoPrimeiro_pen = false;
	golEsquerdoAtivado_pen = false;
	golDireitoAtivado_pen = false;
	golsSomadosTimeEsq_pen = -1;
	golsSomadosTimeDir_pen = -1;
	golsTimeEsq_pen.Init(2, 5);
	golsTimeDir_pen.Init(2, 5);

	playersSpawned = 0;
}

void AOJogoGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOJogoGameState, golsTimeEsq);
	DOREPLIFETIME(AOJogoGameState, golsTimeDir);
	DOREPLIFETIME(AOJogoGameState, bolaEmJogo);
	DOREPLIFETIME(AOJogoGameState, acrescimos);
	DOREPLIFETIME(AOJogoGameState, tempo1Ou2);
	DOREPLIFETIME(AOJogoGameState, tempoRegulamentar);
	DOREPLIFETIME(AOJogoGameState, posInicial);
	DOREPLIFETIME(AOJogoGameState, vsBotIA);
	DOREPLIFETIME(AOJogoGameState, posIndex);
	DOREPLIFETIME(AOJogoGameState, arrayJogadores);
	DOREPLIFETIME(AOJogoGameState, tempoParadoAntesInicioPartida);
	DOREPLIFETIME(AOJogoGameState, tempoParado);
	DOREPLIFETIME(AOJogoGameState, tempoParadoEscanteio);
	DOREPLIFETIME(AOJogoGameState, alturaReinicio);
	DOREPLIFETIME(AOJogoGameState, em_prorrogacao);
	DOREPLIFETIME(AOJogoGameState, penalidades);
	DOREPLIFETIME(AOJogoGameState, timeDireitoPrimeiro_pen);
	DOREPLIFETIME(AOJogoGameState, golEsquerdoAtivado_pen);
	DOREPLIFETIME(AOJogoGameState, golDireitoAtivado_pen);
	DOREPLIFETIME(AOJogoGameState, penalty_timeout);
	DOREPLIFETIME(AOJogoGameState, golsTimeEsq_pen);
	DOREPLIFETIME(AOJogoGameState, golsTimeDir_pen);
	DOREPLIFETIME(AOJogoGameState, golsSomadosTimeEsq_pen);
	DOREPLIFETIME(AOJogoGameState, golsSomadosTimeDir_pen);
	DOREPLIFETIME(AOJogoGameState, playersSpawned);
}

void AOJogoGameState::fazSomApito_Implementation(int32 modo)
{
	if (modo == 0)
	{
		if (som_apito_inicio)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), som_apito_inicio, posInicial);
		else
			UE_LOG(LogTemp, Warning, TEXT("som apito inicio nao encontrado"));
		// GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("som apito inicio nao encontrado")));
	}
	else if (modo == 1)
	{
		if (som_apito_fim)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), som_apito_fim, posInicial);
		else
			UE_LOG(LogTemp, Warning, TEXT("som apito fim nao encontrado"));
		// GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("som apito fim nao encontrado")));
	}
}

void AOJogoGameState::paralisaMovimentacao_Implementation(bool ignora)
{
	for (int32 Index = 0; Index != arrayJogadores.Num(); ++Index)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), Index);
		if (IsValid(PC))
			if (PC->IsMoveInputIgnored() != ignora)
				PC->SetIgnoreMoveInput(ignora);
	}
}

void AOJogoGameState::cutscene_Implementation()
{
	// cutscene
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		ALevelSequenceActor* actor = Cast<ALevelSequenceActor>(FoundActors[0]);
		actor->SequencePlayer->Play();
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("abertura not valid"));
}

void AOJogoGameState::setTempo_Implementation(bool tempo1b, float tempo, AOJogoGameMode* GameModePtr, int32 method)
{
	FTimerHandle* TimerHandler;
	if (tempo1b)
		TimerHandler = &tempo1;
	else
		TimerHandler = &tempo2;
	// GetWorldTimerManager().SetTimer(tempo1, this, &AOJogoGameMode::penaltyTimedOut, tempo, false);
	switch (method)
	{
	case 0:
		GetWorldTimerManager().SetTimer(*TimerHandler, GameModePtr, &AOJogoGameMode::penaltyTimedOut, tempo, false);
		break;
	case 1:
		GetWorldTimerManager().SetTimer(*TimerHandler, GameModePtr, &AOJogoGameMode::maisAcrescimos, tempo, false);
		break;
	case 2:
		GetWorldTimerManager().SetTimer(*TimerHandler, GameModePtr, &AOJogoGameMode::terminaTempo, tempo, false);
		break;
	default:
		break;
	}
}

void AOJogoGameState::openAfterMatch_Implementation()
{
	UAfterMatchWidget* matchResultWidget;

	if (IsValid(widgetClass))
	{
		matchResultWidget = Cast<UAfterMatchWidget>(CreateWidget(GetWorld(), widgetClass));

		if (matchResultWidget)
			matchResultWidget->AddToViewport();
		else
			UE_LOG(LogTemp, Warning, TEXT("nao foi possivel castar widget class nao encontrado"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("after match nao encontrado"));
}