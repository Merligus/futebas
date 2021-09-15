// Fill out your copyright notice in the Description page of Project Settings.


#include "Bola.h"
#include "OJogoGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABola::ABola()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	if (IsValid(RootComponent))
		RootComponent->SetIsReplicated(true);
	PrimaryActorTick.bCanEverTick = false;

	esfera = CreateDefaultSubobject<USphereComponent>(TEXT("Esfera"));
	esfera->InitSphereRadius(32.0f);
	esfera->SetIsReplicated(true);
	RootComponent = esfera;

	aparencia = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Aparencia"));
	aparencia->SetupAttachment(esfera);
	aparencia->SetIsReplicated(true);

	explosao = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Explosao"));
	explosao->SetupAttachment(esfera);
	explosao->SetVisibility(true);
	explosao->SetLooping(false);
	explosao->SetIsReplicated(true);
}

void ABola::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABola, esfera);
	DOREPLIFETIME(ABola, aparencia);
	DOREPLIFETIME(ABola, explosao);
}

// Called when the game starts or when spawned
void ABola::BeginPlay()
{
	Super::BeginPlay();
	// esfera->SetCollisionProfileName(FName(TEXT("BlockAll")), true);
}

// Called every frame
void ABola::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABola::explode_Implementation(FVector posicao)
{
	if (explosao)
	{
		explosao->SetWorldLocation(FVector(posicao.X, posicao.Y + 50, posicao.Z), false, NULL, ETeleportType::TeleportPhysics);
		explosao->PlayFromStart();
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("eplosao nao encontrada")));
}

void ABola::fazSomChute_Implementation(FVector posicao)
{
	if (som_chute)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), som_chute, posicao);
	else
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("som chute nao encontrado")));
}

void ABola::chuta_Implementation(FRotator anguloChute, float forca)
{
	SV_chuta_Implementation(anguloChute, forca);
}

void ABola::SV_chuta_Implementation(FRotator anguloChute, float forca)
{
	fazSomChute(GetActorLocation());
	explode(GetActorLocation());
	MC_chuta(anguloChute, forca);
}

void ABola::MC_chuta_Implementation(FRotator anguloChute, float forca)
{
	esfera->SetCollisionProfileName(FName(TEXT("Ragdoll")), true);
	esfera->AddImpulse(UKismetMathLibrary::GetForwardVector(anguloChute)* forca, FName(TEXT("None")), true);
	esfera->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// esfera->SetCollisionProfileName(FName(TEXT("BlockAll")), true);
}