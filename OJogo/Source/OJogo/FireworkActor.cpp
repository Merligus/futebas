// Fill out your copyright notice in the Description page of Project Settings.


#include "FireworkActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFireworkActor::AFireworkActor()
{
	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Configure character movement
	GetCharacterMovement()->GravityScale = 0.01f;
	GetCharacterMovement()->MaxAcceleration = 10000.0f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->MaxFlySpeed = 5000.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	velocity = 7500.0f;
	rocket_timeout = 2.0f;
	explosion_timeout = 2.0f;
}

// Called when the game starts or when spawned
void AFireworkActor::BeginPlay()
{
	Super::BeginPlay();
	start_position = GetActorLocation();
	
	if (explosions.Num() == 0 || rockets.Num() == 0)
		UE_LOG(LogTemp, Warning, TEXT("Explosions and/or rockets not set"));
}

void AFireworkActor::Fire()
{
	if (invisible)
		GetSprite()->SetFlipbook(invisible);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AFireworkActor::StartAnimation, FMath::RandRange(0.2f, 0.5f), false);
}

void AFireworkActor::StartAnimation()
{
	index_rocket = FMath::RandRange(0, rockets.Num()-1);
	index_explosion = FMath::RandRange(0, explosions.Num()-1);
	RootComponent->SetWorldLocation(start_position, false, NULL, ETeleportType::TeleportPhysics);

	if (index_rocket < rockets.Num())
		GetSprite()->SetFlipbook(rockets[index_rocket]);

	LaunchCharacter(velocity * FVector(GetActorForwardVector()).GetSafeNormal(), true, true);

	rocket_timeout = FMath::RandRange(1.5f, 2.0f);

	if (rocket_sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), rocket_sound, start_position);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AFireworkActor::UpdateAnimation, rocket_timeout, false);
}

void AFireworkActor::UpdateAnimation()
{
	LaunchCharacter((-0.1f) * FVector(GetActorForwardVector()).GetSafeNormal(), true, true);

	if (index_explosion < explosions.Num())
		GetSprite()->SetFlipbook(explosions[index_explosion]);

	if (explosion_sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosion_sound, start_position);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AFireworkActor::EndAnimation, explosion_timeout, false);
}

void AFireworkActor::EndAnimation()
{
	if (invisible)
		GetSprite()->SetFlipbook(invisible);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AFireworkActor::StartAnimation, FMath::RandRange(0.2f, 2.2f), false);
}