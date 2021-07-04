// Fill out your copyright notice in the Description page of Project Settings.


#include "PartidasData.h"

void FPartidasData::AddNewRow(int32 index_time)
{
    casa.Add(index_time, FPartidasLinhaData());
}

void FPartidasData::AddUninitialized(TArray<int32> times)
{
    for (auto& index_time : times)
        AddNewRow(index_time);

    for (auto& index_time1 : times)
        for (auto& index_time2 : times)
            casa[index_time1].AddNewColumn(index_time2);
}
