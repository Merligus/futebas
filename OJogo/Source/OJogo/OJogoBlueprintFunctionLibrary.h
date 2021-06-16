// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OJogoBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API UOJogoBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		UFUNCTION(BlueprintCallable, Category = "Futebas")
		static bool GetTeamData(FString teamName);
};
