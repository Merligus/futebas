// Fill out your copyright notice in the Description page of Project Settings.


#include "PartidasData.h"

void FPartidasData::AddNewRow()
{
    casa.Add(FPartidasLinhaData());
}

void FPartidasData::AddUninitialized(const int32 RowCount, const int32 ColCount)
{
    //Add Rows
    for(int32 v = 0; v < RowCount; v++)
        AddNewRow();
    
    //Add Columns
    for(int32 v = 0; v < RowCount; v++)
        for(int32 b = 0; b < ColCount; b++)
            casa[v].AddNewColumn();
}
