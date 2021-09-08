// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterState.h"
#include "OJogoCharacter.h"

APlayerCharacterState::APlayerCharacterState()
{
    stamina = 100.0f;
	forca_chute = 0.0f;

    /*AOJogoCharacter* Char(GetPawn<AOJogoCharacter>());
    if (Char)
    {
        Char->SetActorTickEnabled(true);
        Char->PrimaryActorTick.bCanEverTick = true;
        Char->PrimaryActorTick.SetTickFunctionEnable(true);
        Char->PrimaryActorTick.bStartWithTickEnabled = true;
    }*/
}

void APlayerCharacterState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacterState, stamina);
    DOREPLIFETIME(APlayerCharacterState, forca_chute);
}

void APlayerCharacterState::SV_SetForcaChute_Implementation(float forca)
{
    forca_chute = forca;
}

float APlayerCharacterState::GetForcaChute() const
{
    return forca_chute;
}

void APlayerCharacterState::SV_SetStamina_Implementation(float stam)
{
    stamina = stam;
}

float APlayerCharacterState::GetStamina() const
{
    return stamina;
}