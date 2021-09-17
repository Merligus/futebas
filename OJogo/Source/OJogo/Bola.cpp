// Fill out your copyright notice in the Description page of Project Settings.


#include "Bola.h"
#include "OJogoGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABola::ABola()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bola = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bola"));
	bola->SetIsReplicated(false);

	aparencia = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Aparencia"));
	aparencia->SetRelativeLocation(FVector(0, 0, -50));
	aparencia->SetIsReplicated(false);
	aparencia->SetupAttachment(bola);

	esfera = CreateDefaultSubobject<USphereComponent>(TEXT("Esfera"));
	esfera->InitSphereRadius(50.0f);
	esfera->SetIsReplicated(false);
	esfera->SetupAttachment(bola);

	explosao = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Explosao"));
	explosao->SetRelativeScale3D(FVector(2.5, 2.5, 2.5));
	explosao->SetVisibility(true);
	explosao->SetLooping(false);
	explosao->SetIsReplicated(false);
	explosao->SetupAttachment(bola);

	RootComponent = bola;
}

void ABola::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts or when spawned
void ABola::BeginPlay()
{
	Super::BeginPlay();

	Cast<UStaticMeshComponent>(GetRootComponent())->SetCollisionProfileName(FName(TEXT("Ragdoll")), true);
	Cast<UStaticMeshComponent>(GetRootComponent())->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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

void ABola::SV_chuta_Implementation(FRotator anguloChute, float forca)
{
	fazSomChute(GetActorLocation());
	explode(GetActorLocation());
	Cast<UStaticMeshComponent>(GetRootComponent())->AddImpulse(UKismetMathLibrary::GetForwardVector(anguloChute) * forca, FName(TEXT("None")), true);
}