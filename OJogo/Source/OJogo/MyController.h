// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "MyController.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API AMyController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* aPawn) override;

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void RPC_PossessRequest();
	void RPC_PossessRequest_Implementation();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void PossessRequest(APlayerController* PC);
	void PossessRequest_Implementation(APlayerController* PC);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void SetCamera();
	void SetCamera_Implementation();
};
