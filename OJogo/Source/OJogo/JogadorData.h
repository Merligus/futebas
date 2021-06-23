// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "UObject/NoExportTypes.h"
#include "JogadorData.generated.h"

/**
 *
UCLASS()
class OJOGO_API UJogadorData : public UObject
{
	GENERATED_BODY()

};
*/

USTRUCT(BlueprintType)
struct FJogadorData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
	TArray<class UPaperFlipbook*> cabeloArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
	TArray<class UPaperFlipbook*> olhoArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
	TArray<class UPaperFlipbook*> luvaArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
	TArray<class UPaperFlipbook*> peleArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
	TArray<class UPaperFlipbook*> roupa1Array;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
	TArray<class UPaperFlipbook*> roupa2Array;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbook")
	TArray<class UPaperFlipbook*> chuteiraArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor cabelo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor olho;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor luva;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor pele;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor roupa1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor roupa2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
	FLinearColor chuteira;

	FJogadorData()
	{

	}

	~FJogadorData()
	{

	}
};