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

	gols_penalti_casa = -1;
	index_casa = -1;
	
	gols_penalti_fora = -1;
	index_fora = -1;
}

FResultadoData::FResultadoData(int32 g_casa, int32 g_fora)
{
    gols_casa = g_casa;
    gols_fora = g_fora;

	gols_penalti_casa = -1;
	index_casa = -1;
	
	gols_penalti_fora = -1;
	index_fora = -1;
}

FResultadoData::FResultadoData(int32 g_casa, int32 g_casa_pen, int32 g_fora, int32 g_fora_pen)
{
    gols_casa = g_casa;
    gols_fora = g_fora;

	gols_penalti_casa = g_casa_pen;
	gols_penalti_fora = g_fora_pen;

	index_casa = -1;
	index_fora = -1;
}

int32 FResultadoData::getGanhador() const
{
	if (gols_casa > gols_fora)
		return index_casa;
	else if (gols_fora > gols_casa)
		return index_fora;
	else
		if (gols_penalti_casa > gols_penalti_fora)
			return index_casa;
		else if (gols_penalti_casa < gols_penalti_fora)
			return index_fora;
		else
			return -1;
}