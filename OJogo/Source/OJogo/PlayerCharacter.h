// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OJogoCharacter.h"
#include "PauseWidget.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API APlayerCharacter : public AOJogoCharacter
{
	GENERATED_BODY()

	UPROPERTY()
	FTimerHandle chargeHandle;

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(VisibleInstanceOnly)
	class UPauseWidget* pauseWidget;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void colocadoPressed();

	UFUNCTION(BlueprintCallable)
	void colocadoReleased();

	UFUNCTION(BlueprintCallable)
	void cavadoReleased();

	UFUNCTION(BlueprintCallable)
	void charge();

	UFUNCTION(BlueprintCallable)
	void chargeTimeOut();

	UFUNCTION(BlueprintCallable)
	void pause();
};
