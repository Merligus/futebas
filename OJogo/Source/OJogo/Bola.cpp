// Fill out your copyright notice in the Description page of Project Settings.


#include "Bola.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABola::ABola()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	esfera = CreateDefaultSubobject<USphereComponent>(TEXT("Esfera"));
	esfera->InitSphereRadius(32.0f);
	RootComponent = esfera;

	aparencia = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Aparencia"));
	aparencia->SetupAttachment(esfera);

	explosao = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Explosao"));
	aparencia->SetupAttachment(esfera);
}

// Called when the game starts or when spawned
void ABola::BeginPlay()
{
	Super::BeginPlay();
	AOJogoGameState* JogosGameState = GetWorld()->GetAuthGameMode()->GetGameState<AOJogoGameState>();
	JogosGameState->posInicial = GetActorLocation();
	esfera->SetCollisionProfileName(FName(TEXT("BlockAll")), true);
}

// Called every frame
void ABola::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABola::explode(FVector posicao)
{
	// if (explosao)
	// 	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), explosao, posicao, FRotator(0), FVector(1), true, true, ENCPoolMethod::None, true);
	// else
	// 	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("eplosao nao encontrada")));
}

void ABola::chuta(FRotator anguloChute, float forca)
{
	// explode(GetActorLocation());

	esfera->SetCollisionProfileName(FName(TEXT("Ragdoll")), true);
	esfera->AddImpulse(UKismetMathLibrary::GetForwardVector(anguloChute) * forca, FName(TEXT("None")), true);
	esfera->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	esfera->SetCollisionProfileName(FName(TEXT("BlockAll")), true);
}