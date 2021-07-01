// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JogadorData.h"
#include "HabilidadesData.h"
#include "Engine/Texture2D.h"
#include "TeamData.generated.h"

/**

UCLASS()
class OJOGO_API UTeamData : public UObject
{
	GENERATED_BODY()
	
};
 * 
 */


USTRUCT(BlueprintType)
struct FTeamData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FString nome_interno;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FString nome_hud;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FString sigla;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	UTexture2D* flag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FHabilidadesData habilidades;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FName estadio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FJogadorData jogador;

	FTeamData() { }

	~FTeamData() { }
};