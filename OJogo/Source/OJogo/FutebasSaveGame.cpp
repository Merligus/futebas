// Fill out your copyright notice in the Description page of Project Settings.


#include "FutebasSaveGame.h"

UFutebasSaveGame::UFutebasSaveGame()
{
    copa_do_mundo = FCopaMundoData();
    liga_das_nacoes = FLigaData(16, 32);
    liga_das_nacoes.alocarTimes();
}