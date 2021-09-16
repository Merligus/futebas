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
		RootComponent->SetIsReplicated(false);
	PrimaryActorTick.bCanEverTick = false;

	esfera = CreateDefaultSubobject<USphereComponent>(TEXT("Esfera"));
	esfera->InitSphereRadius(32.0f);
	esfera->SetIsReplicated(false);
	RootComponent = esfera;

	aparencia = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Aparencia"));
	aparencia->SetupAttachment(esfera);
	aparencia->SetIsReplicated(false);

	explosao = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Explosao"));
	explosao->SetupAttachment(esfera);
	explosao->SetVisibility(true);
	explosao->SetLooping(false);
	explosao->SetIsReplicated(false);
}

void ABola::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts or when spawned
void ABola::BeginPlay()
{
	Super::BeginPlay();

	esfera->SetCollisionProfileName(FName(TEXT("Ragdoll")), true);
	esfera->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called every frame
void ABola::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HasAuthority())
		UpdateBall(GetActorLocation(), esfera->GetPhysicsLinearVelocity());
}

void ABola::UpdateBall_Implementation(FVector posicao, FVector velocidade)
{
	if (!HasAuthority())
	{
		current_vel = esfera->GetPhysicsLinearVelocity();
		current_pos = GetActorLocation();

		esfera->SetPhysicsLinearVelocity(velocidade);

		if (UKismetMathLibrary::VSize(posicao - current_pos) > 10.0f)
		{
			FVector t(0);

			if (velocidade.X != 0)
				t.X = (posicao - current_pos).X / velocidade.X;

			if (velocidade.Y != 0)
				t.Y = (posicao - current_pos).Y / velocidade.Y;

			if (velocidade.Z != 0)
				t.Z = (posicao - current_pos).Z / velocidade.Z;

			FVector F;
			F = t * velocidade;
			esfera->AddImpulse(F, FName(TEXT("None")), true);

			GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, FString::Printf(TEXT("V = %.2f %.2f %.2f"), F.X, F.Y, F.Z));
		}
	}
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
	esfera->AddImpulse(UKismetMathLibrary::GetForwardVector(anguloChute) * forca, FName(TEXT("None")), true);
}