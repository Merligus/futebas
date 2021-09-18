// Fill out your copyright notice in the Description page of Project Settings.


#include "MyController.h"
#include "Kismet/GameplayStatics.h"
#include "OJogoGameMode.h"
#include "Camera/CameraActor.h"

void AMyController::BeginPlay()
{
    Super::BeginPlay();
    
    RPC_PossessRequest();
}

void AMyController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);

    SetCamera();
}

void AMyController::RPC_PossessRequest_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Request"));
    int32 team;
    if (HasAuthority())
        team = 0;
    else
        team = 1;

    PossessRequest(this, team);
}

void AMyController::PossessRequest_Implementation(APlayerController* PC, int32 team)
{
    AOJogoGameMode* GM = Cast<AOJogoGameMode>(GetWorld()->GetAuthGameMode());

    if (IsValid(GM))
        GM->possessRequested(this, team);
}

void AMyController::SetCamera_Implementation()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

    for (int32 i = 0; i < FoundActors.Num(); ++i)
        if (FoundActors[i]->Tags.Num() > 0)
            if (FoundActors[i]->Tags[0].Compare(TEXT("player")) == 0)
            {
                UE_LOG(LogTemp, Warning, TEXT("Camera"));
                SetViewTargetWithBlend(FoundActors[i]);
            }
}