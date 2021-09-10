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
    SetCamera();
}

void AMyController::RPC_PossessRequest_Implementation()
{
    GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Request")));
    UE_LOG(LogTemp, Warning, TEXT("Request"));
    PossessRequest(this);
}

void AMyController::PossessRequest_Implementation(APlayerController* PC)
{
    AOJogoGameMode* GM = Cast<AOJogoGameMode>(GetWorld()->GetAuthGameMode());

    if (IsValid(GM))
        GM->possessRequested(this);
}

void AMyController::SetCamera_Implementation()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

    for (int32 i = 0; i < FoundActors.Num(); ++i)
        if (FoundActors[i]->Tags.Num() > 0)
        {
            GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, FString::Printf(TEXT("Camera")));
            UE_LOG(LogTemp, Warning, TEXT("Camera"));
            SetViewTargetWithBlend(FoundActors[i]);
        }
}
