// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamData.h"

FTeamData::FTeamData()
{
    index_time = -1;
    flag = UTexture2D::CreateTransient(12, 12, PF_R8G8B8A8_UINT, FName(TEXT("TeamFlagVoid")));
}