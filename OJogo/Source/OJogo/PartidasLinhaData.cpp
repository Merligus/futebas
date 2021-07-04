// Fill out your copyright notice in the Description page of Project Settings.


#include "PartidasLinhaData.h"

void FPartidasLinhaData::AddNewColumn(int32 index_time)
{
    fora.Add(index_time, FResultadoData());
}