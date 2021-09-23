// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "OJogoGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

ABall::ABall()
{
	explosao = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Explosao"));
	explosao->SetRelativeScale3D(FVector(2.5, 2.5, 2.5));
	explosao->SetVisibility(true);
	explosao->SetLooping(false);
	explosao->SetIsReplicated(false);
	explosao->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		AOJogoGameState* JogosGameState = GetWorld()->GetAuthGameMode()->GetGameState<AOJogoGameState>();
		if (IsValid(JogosGameState))
			JogosGameState->posInicial = GetActorLocation();
	}
}

void ABall::explode_Implementation(FVector posicao)
{
	if (explosao)
	{
		explosao->SetWorldLocation(FVector(posicao.X, posicao.Y + 50, posicao.Z), false, NULL, ETeleportType::TeleportPhysics);
		explosao->PlayFromStart();
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("eplosao nao encontrada"));
}

void ABall::fazSomChute_Implementation(FVector posicao)
{
	if (som_chute)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), som_chute, posicao);
	else
		UE_LOG(LogTemp, Warning, TEXT("som chute nao encontrado"));
}

void ABall::SV_chuta_Implementation(FRotator anguloChute, float forca)
{
	fazSomChute(GetActorLocation());
	explode(GetActorLocation());
	Cast<UStaticMeshComponent>(GetRootComponent())->AddImpulse(UKismetMathLibrary::GetForwardVector(anguloChute) * forca, FName(TEXT("None")), true);
}