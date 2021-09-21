// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OJogoCharacter.h"
#include "PauseWidget.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum DirecaoChute
{
	Esquerda, 
	Direita, 
	Cima, 
	Baixo
};

UCLASS()
class OJOGO_API APlayerCharacter : public AOJogoCharacter
{
	GENERATED_BODY()

	UPROPERTY()
	FTimerHandle chargeHandle;

	UPROPERTY()
	TArray<TEnumAsByte<DirecaoChute>> ordem_direcao;

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(VisibleInstanceOnly)
	class UPauseWidget* pauseWidget;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	UPROPERTY(VisibleAnywhere)
	bool pausable;

public:

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void PulaP2();

	UFUNCTION(BlueprintCallable)
	void chutaPressedP2();

	UFUNCTION(BlueprintCallable)
	void chutaReleasedP2();

	UFUNCTION(BlueprintCallable)
	void DirecaoHorizontalP2(float Value);

	UFUNCTION(BlueprintCallable)
	void DirecaoVerticalP2(float Value);

	UFUNCTION(BlueprintCallable)
	void PlayerSlideP2(float Value);

	UFUNCTION(BlueprintCallable)
	void DirecaoHorizontal(float Value);

	UFUNCTION(BlueprintCallable)
	void DirecaoVertical(float Value);

	UFUNCTION(BlueprintCallable)
	void PlayerSlide(float Value);

	UFUNCTION(BlueprintCallable)
	void chutaPressed();

	UFUNCTION(BlueprintCallable)
	void chutaReleased();

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

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetUnpausable();
	void SetUnpausable_Implementation();
};
