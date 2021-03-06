// Fill out your copyright notice in the Description page of Project Settings.


#include "FutebasSaveGame.h"

UFutebasSaveGame::UFutebasSaveGame()
{
    volumes.Init(1.0, 3);
	CDM = false;
	LDN = false;
	CMC = false;
	LMC = false;
    gols_unica_copa = 0;
	copas = 0;
	gols_todas_copas = 0;
}

FCopaMundoData* UFutebasSaveGame::GetCopa(int32 index)
{
    while(index >= copa_do_mundo.Num())
        copa_do_mundo.Add(FCopaMundoData());
    return &copa_do_mundo[index];
}

FLigaData* UFutebasSaveGame::GetLiga(int32 index)
{
    while(index >= liga_das_nacoes.Num())
        liga_das_nacoes.Add(FLigaData());
    return &liga_das_nacoes[index];
}

void UFutebasSaveGame::SetCopa(FCopaMundoData copa, int32 index)
{
    while(index >= copa_do_mundo.Num())
        copa_do_mundo.Add(FCopaMundoData());
    copa_do_mundo[index] = copa;
}

void UFutebasSaveGame::SetLiga(FLigaData liga, int32 index)
{
    while(index >= liga_das_nacoes.Num())
        liga_das_nacoes.Add(FLigaData());
    liga_das_nacoes[index] = liga;
}