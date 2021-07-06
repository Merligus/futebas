// Fill out your copyright notice in the Description page of Project Settings.


#include "MataMataData.h"

FFase::FFase(int32 fase)
{
    int32 n_confrontos = FMath::Pow(2, 4-fase);
    for (int32 i = 0; i < n_confrontos; ++i)
        confrontos.Add(FResultadoData());
}

void FMataMataData::chaveia(int32 fase)
{
    fases.Add(FFase(fase));
}