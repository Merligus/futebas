// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacterState.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API APlayerCharacterState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float forca_chute;

public:

	APlayerCharacterState();

	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void SV_SetForcaChute(float forca);
	void SV_SetForcaChute_Implementation(float forca);

	UFUNCTION(BlueprintCallable)
	float GetForcaChute() const;

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void SV_SetStamina(float stam);
	void SV_SetStamina_Implementation(float stam);

	UFUNCTION(BlueprintCallable)
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void staminaRegenLoop();
	void staminaRegenLoop_Implementation();
	
};
