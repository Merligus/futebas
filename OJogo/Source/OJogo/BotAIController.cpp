// Fill out your copyright notice in the Description page of Project Settings.


#include "BotAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

ABotAIController::ABotAIController()
{
    BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void ABotAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ABotCharacter* BotIA = Cast<ABotCharacter>(InPawn);

    if (BotIA)
    {
        if (BotIA->GetBehaviorTree()->BlackboardAsset)
            BlackboardComp->InitializeBlackboard(*(BotIA->GetBehaviorTree()->BlackboardAsset));
        else
            UE_LOG(LogTemp, Warning, TEXT("Sem BlackboardAsset"));
        
        BehaviorComp->StartTree(*BotIA->GetBehaviorTree());
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("Sem iniciar BT"));
}