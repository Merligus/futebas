// Fill out your copyright notice in the Description page of Project Settings.


#include "OJogoGameState.h"

AOJogoGameState::AOJogoGameState()
{
	botIA_c = LoadClass<AOJogoCharacter>(NULL, TEXT("/Game/Teams/IA/Bot.Bot_C"), NULL, 0, NULL);

    golsTimeEsq = 0;
	golsTimeDir = 0;
	bolaEmJogo = true;
	acrescimos = FTimespan(0, 0, 0);
	tempo1Ou2 = 1;
	tempoRegulamentar = true;
	posInicial = FVector(0.0f);
	vsBotIA = true;
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
}