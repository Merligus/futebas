// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamData.h"
#include "CampeonatoData.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "FutebasGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UFutebasGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()	

	UPROPERTY()
    TArray<FTeamData> teamsArray;

	UPROPERTY()
    TMap<int32, int32> sorteioGrupo;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData team1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData team2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCampeonatoData> tabelaGrupos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* teams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float volume;

	void loadTeams(int32 grupos);

	void sortear(int32 grupos);

	FTeamData getTeam(int32 index);

	void bindIndexTimeGrupos();
};
