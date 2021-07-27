// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FutebasGameInstance.h"
#include "PartidaSlotWidget.h"
#include "Components/VerticalBox.h"
#include "Components/SpinBox.h"
#include "Blueprint/UserWidget.h"
#include "RodadaPartidasWidget.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API URodadaPartidasWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UFutebasGameInstance* FutebasGI;

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(VisibleInstanceOnly)
	class UPartidaSlotWidget* partidaSlotWidget;

	UPROPERTY(VisibleInstanceOnly)
	TArray<class UPartidaSlotWidget*> partidasSlots;

    virtual bool Initialize();

	virtual void NativeConstruct() override;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	GameMode game_mode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	int32 grupo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TeamsSet teams_set;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UVerticalBox* PartidasBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	USpinBox* rodadaSlider;

	UFUNCTION(BlueprintCallable)
	void SliderValue(float InValue);
	
};
