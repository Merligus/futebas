// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(widgetClass))
		pauseWidget = Cast<UPauseWidget>(CreateWidget(GetWorld(), widgetClass));
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		else if (TravelDirection > 0.0f)
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
        else if (bisMovingRight)
            Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
        else
            Controller->SetControlRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &AOJogoCharacter::slideAction);
    PlayerInputComponent->BindAction("Slide", IE_Released, this, &AOJogoCharacter::stopSliding);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AOJogoCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Colocado", IE_Pressed, this, &APlayerCharacter::colocadoPressed);
	PlayerInputComponent->BindAction("Colocado", IE_Released, this, &APlayerCharacter::colocadoReleased);
	PlayerInputComponent->BindAction("Cavado", IE_Pressed, this, &APlayerCharacter::colocadoPressed);
	PlayerInputComponent->BindAction("Cavado", IE_Released, this, &APlayerCharacter::cavadoReleased);
	PlayerInputComponent->BindAction("Pause", IE_Released, this, &APlayerCharacter::pause);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOJogoCharacter::MoveRight);
}

void APlayerCharacter::colocadoPressed()
{
	forca_chute = 0.0f;
	GetWorldTimerManager().SetTimer(chargeHandle, this, &APlayerCharacter::charge, 0.005f, true);
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::chargeTimeOut, 0.5f, false);
}

void APlayerCharacter::colocadoReleased()
{
    Getchute_angulo()->SetRelativeRotation(FRotator(0, 0, 0));
	chargeTimeOut();
	Chuta();
}

void APlayerCharacter::cavadoReleased()
{
	Getchute_angulo()->SetRelativeRotation(FRotator(15, 0, 0));
	chargeTimeOut();
	Chuta();
}

void APlayerCharacter::charge()
{
	forca_chute += 0.04;
	if (forca_chute > 1)
	{
		forca_chute = 1.0f;
		chargeTimeOut();
		Chuta();
	}
}

void APlayerCharacter::chargeTimeOut()
{
	GetWorldTimerManager().ClearTimer(chargeHandle);
}

void APlayerCharacter::pause()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		if (pauseWidget)
			pauseWidget->RemoveFromParent();
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PC->SetShowMouseCursor(false);
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		if (pauseWidget)
			pauseWidget->AddToViewport();
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PC->SetShowMouseCursor(true);
	}
}