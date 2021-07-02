// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamData.h"
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

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData team1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData team2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* teams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float volume;

	void loadTeams();

	FTeamData getTeam(int32 index);
};
