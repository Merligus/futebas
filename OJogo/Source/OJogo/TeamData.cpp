// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamData.h"
#include "Engine/ObjectLibrary.h"

FTeamData::FTeamData()
{
    index_time = -1;
    UObjectLibrary* ObjectLibrary;
    ObjectLibrary = UObjectLibrary::CreateLibrary(UTexture2D::StaticClass(), false, GIsEditor);
    ObjectLibrary->AddToRoot();
    ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game/Teams/flags"));
    ObjectLibrary->LoadAssetsFromAssetData();

    TArray<UTexture2D*> AllFlags;

    ObjectLibrary->GetObjects(AllFlags);
    if (AllFlags.Num() > 0)
        flag = AllFlags[0];
    else
        UE_LOG(LogTemp, Warning, TEXT("Nenhuma flag encontrada"));
}