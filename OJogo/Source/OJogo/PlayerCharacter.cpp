// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	PlayerInputComponent->BindAction("Chute", IE_Pressed, this, &APlayerCharacter::chutaPressed);
	PlayerInputComponent->BindAction("Chute", IE_Released, this, &APlayerCharacter::chutaReleased);
    // PlayerInputComponent->BindAction("Colocado", IE_Pressed, this, &APlayerCharacter::colocadoPressed);
	// PlayerInputComponent->BindAction("Colocado", IE_Released, this, &APlayerCharacter::colocadoReleased);
	// PlayerInputComponent->BindAction("Cavado", IE_Pressed, this, &APlayerCharacter::colocadoPressed);
	// PlayerInputComponent->BindAction("Cavado", IE_Released, this, &APlayerCharacter::cavadoReleased);
	PlayerInputComponent->BindAction("Pause", IE_Released, this, &APlayerCharacter::pause);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::DirecaoHorizontal);
	PlayerInputComponent->BindAxis("DirecaoVertical", this, &APlayerCharacter::DirecaoVertical);
}

void APlayerCharacter::DirecaoHorizontal(float Value)
{
	if (Value > 0)
		ordem_direcao.AddUnique(DirecaoChute::Direita);
	else if (Value < 0)
		ordem_direcao.AddUnique(DirecaoChute::Esquerda);
	else
	{
		ordem_direcao.Remove(DirecaoChute::Esquerda);
		ordem_direcao.Remove(DirecaoChute::Direita);
	}
	MoveRight(Value);
}

void APlayerCharacter::DirecaoVertical(float Value)
{
	if (Value > 0)
		ordem_direcao.AddUnique(DirecaoChute::Cima);
	else if (Value < 0)
		ordem_direcao.AddUnique(DirecaoChute::Baixo);
	else
	{
		ordem_direcao.Remove(DirecaoChute::Baixo);
		ordem_direcao.Remove(DirecaoChute::Cima);
	}
}

void APlayerCharacter::chutaPressed()
{
	forca_chute = 0.0f;
	GetWorldTimerManager().SetTimer(chargeHandle, this, &APlayerCharacter::charge, 0.005f, true);
}

void APlayerCharacter::chutaReleased()
{
	float supress_mult(0.4);
	float angulo, x_temp(0), y_temp(0);
	if (ordem_direcao.Num() > 0)
	{
		for (int32 index = 0; index < ordem_direcao.Num(); ++index)
		{
			if (ordem_direcao[index] == DirecaoChute::Esquerda)
				x_temp -= 1;
			else if (ordem_direcao[index] == DirecaoChute::Direita)
				x_temp += 1;
			else if (ordem_direcao[index] == DirecaoChute::Cima)
				y_temp += 1;
			else
				y_temp -= 1;
		}
		if (ordem_direcao[0] == DirecaoChute::Baixo || ordem_direcao[0] == DirecaoChute::Cima)
			x_temp *= supress_mult;
		else if (ordem_direcao[0] == DirecaoChute::Esquerda || ordem_direcao[0] == DirecaoChute::Direita)
			y_temp *= supress_mult;
		
		angulo = UKismetMathLibrary::DegAtan(y_temp/(x_temp + 0.000000000001));
		angulo += (x_temp < 0)? 180 : 0;
		// if ((x < 0 && bisMovingRight) || (x > 0 && !bisMovingRight))
	}
	else
		angulo = bisMovingRight? 0 : 180;
	
	// GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("angulo %.1f, x=%.1f y=%.1f"), angulo, x_temp, y_temp));
	Getchute_angulo()->SetWorldRotation(FRotator(angulo, 0, 0));
	chargeTimeOut();
	Chuta();
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
	forca_chute += 0.02;
	if (forca_chute > 1)
	{
		forca_chute = 1.0f;
		// chargeTimeOut();
		// Chuta();
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