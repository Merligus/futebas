// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultadoData.h"

FResultadoData::FResultadoData()
{
    gols_casa = -1;
    gols_fora = -1;
}

FResultadoData::FResultadoData(int32 empate)
{
    gols_casa = gols_fora = empate;
}

FResultadoData::FResultadoData(int32 casa, int32 fora)
{
    gols_casa = casa;
    gols_fora = fora;
}