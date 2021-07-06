// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultadoData.h"

FResultadoData::FResultadoData()
{
	gols_casa = -1;
	gols_penalti_casa = -1;
	index_casa = -1;

	gols_fora = -1;
	gols_penalti_fora = -1;
	index_fora = -1;
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

int32 FResultadoData::getGanhador()
{
	if (gols_casa > gols_fora)
		return index_casa;
	else if (gols_fora > gols_casa)
		return index_fora;
	else
		if (gols_penalti_casa > gols_penalti_fora)
			return index_casa;
		else
			return index_fora;
}