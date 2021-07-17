// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BotAIController.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API ABotAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY()
	UBlackboardComponent* BlackboardComp;

	virtual void OnPossess(APawn* InPawn) override;

public:

	ABotAIController();

	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
	
};
