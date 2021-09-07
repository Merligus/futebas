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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	float stamina;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	float forca_chute;

public:

	APlayerCharacterState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server, Unreliable)
	void SetForcaChute(float forca);
	void SetForcaChute_Implementation(float forca);

	UFUNCTION(NetMulticast, Unreliable)
	void MC_SetForcaChute(float forca);
	void MC_SetForcaChute_Implementation(float forca);

	float GetForcaChute() const;

	UFUNCTION(Server, Unreliable)
	void SetStamina(float stam);
	void SetStamina_Implementation(float stam);

	UFUNCTION(NetMulticast, Unreliable)
	void MC_SetStamina(float stam);
	void MC_SetStamina_Implementation(float stam);

	float GetStamina() const;
	
};
