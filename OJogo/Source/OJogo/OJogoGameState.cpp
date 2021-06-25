// Fill out your copyright notice in the Description page of Project Settings.


#include "OJogoGameState.h"

AOJogoGameState::AOJogoGameState()
{
    // static ConstructorHelpers::FObjectFinder<UClass> bolaClassFinder( TEXT("Blueprint'/Game/Items/Ball/Ball.Ball_C'") );
	// const TCHAR* PreviewPlayer1Name("Blueprint'/Game/Character/Player1.Player1'");
	Bola_c = LoadObject<UClass>(NULL, TEXT("/Game/Items/Ball/Ball.Ball_C"), NULL, 0, NULL);
	// player1_c = LoadClass<AOJogoCharacter>(NULL, *PreviewPlayer1Name, NULL, 0, NULL);
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
}