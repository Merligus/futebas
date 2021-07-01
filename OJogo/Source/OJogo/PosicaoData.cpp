// Fill out your copyright notice in the Description page of Project Settings.


#include "PosicaoData.h"

FPosicaoData::FPosicaoData()
{
    pontos = 0;
	partidas_jogadas = 0;
	vitorias = 0;
	empates = 0;
	derrotas = 0;
	gols_pro = 0;
	gols_contra = 0;
	saldo_de_gols = 0;
	index_time = 0;
}

FPosicaoData::FPosicaoData(int32 index)
{
    pontos = 0;
	partidas_jogadas = 0;
	vitorias = 0;
	empates = 0;
	derrotas = 0;
	gols_pro = 0;
	gols_contra = 0;
	saldo_de_gols = 0;
	index_time = index;
}
