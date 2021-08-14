// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerStart.h"

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(widgetClass))
		pauseWidget = Cast<UPauseWidget>(CreateWidget(GetWorld(), widgetClass));

	pausable = true;
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	for (int32 Index = 0; Index < PlayerStarts.Num(); ++Index)
	{
		APlayerStart* PS = Cast<APlayerStart>(PlayerStarts[Index]);
		if (IsValid(PS))
		{
			FString tag = PS->PlayerStartTag.ToString();
			int32 indicePS = FCString::Atoi(*tag);
			if (index_controller == indicePS)
			{
				FTransform NewLocation(PS->GetActorTransform());
				SetActorTransform(NewLocation, false, NULL, ETeleportType::TeleportPhysics);
			}
		}
	}
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
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (index_controller == 0 && pausable)
	{
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AOJogoCharacter::Pula);
		PlayerInputComponent->BindAction("Chute", IE_Pressed, this, &APlayerCharacter::chutaPressed);
		PlayerInputComponent->BindAction("Chute", IE_Released, this, &APlayerCharacter::chutaReleased);
		PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCharacter::pause).bExecuteWhenPaused = true;
		PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::DirecaoHorizontal);
		PlayerInputComponent->BindAxis("DirecaoVertical", this, &APlayerCharacter::DirecaoVertical);
		PlayerInputComponent->BindAxis("Slide", this, &APlayerCharacter::PlayerSlide);

		PlayerInputComponent->BindAction("Jump2", IE_Pressed, this, &APlayerCharacter::PulaP2);
		PlayerInputComponent->BindAction("Chute2", IE_Pressed, this, &APlayerCharacter::chutaPressedP2);
		PlayerInputComponent->BindAction("Chute2", IE_Released, this, &APlayerCharacter::chutaReleasedP2);
		PlayerInputComponent->BindAxis("MoveRight2", this, &APlayerCharacter::DirecaoHorizontalP2);
		PlayerInputComponent->BindAxis("DirecaoVertical2", this, &APlayerCharacter::DirecaoVerticalP2);
		PlayerInputComponent->BindAxis("Slide2", this, &APlayerCharacter::PlayerSlideP2);
	}
}

void APlayerCharacter::PulaP2()
{
	APlayerCharacter* player2 = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), index_controller+1));
	UE_LOG(LogTemp, Warning, TEXT("calling P2 Pula"));
	if (IsValid(player2) && player2 != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("calling P2 Pula"));
		player2->Pula();
	}
}

void APlayerCharacter::chutaPressedP2()
{
	APlayerCharacter* player2 = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), index_controller+1));
	UE_LOG(LogTemp, Warning, TEXT("calling P2 chutaPressed"));
	if (IsValid(player2) && player2 != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("calling P2 chutaPressed"));
		player2->chutaPressed();
	}
}

void APlayerCharacter::chutaReleasedP2()
{
	APlayerCharacter* player2 = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), index_controller+1));
	UE_LOG(LogTemp, Warning, TEXT("calling P2 chutaReleased"));
	if (IsValid(player2) && player2 != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("calling P2 chutaReleased"));
		player2->chutaReleased();
	}
}

void APlayerCharacter::DirecaoHorizontalP2(float Value)
{
	APlayerCharacter* player2 = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), index_controller+1));
	if (IsValid(player2) && player2 != this)
	{
		if (Value != 0)
			UE_LOG(LogTemp, Warning, TEXT("calling P2 DirecaoHorizontal"));
		player2->DirecaoHorizontal(Value);
	}
}

void APlayerCharacter::DirecaoVerticalP2(float Value)
{
	APlayerCharacter* player2 = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), index_controller+1));
	if (IsValid(player2) && player2 != this)
	{
		if (Value != 0)
			UE_LOG(LogTemp, Warning, TEXT("calling P2 DirecaoVertical"));
		player2->DirecaoVertical(Value);
	}
}

void APlayerCharacter::PlayerSlideP2(float Value)
{
	APlayerCharacter* player2 = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), index_controller+1));
	if (IsValid(player2) && player2 != this)
	{
		if (Value != 0)
			UE_LOG(LogTemp, Warning, TEXT("calling P2 PlayerSlide"));
		player2->PlayerSlide(Value);
	}
}

void APlayerCharacter::DirecaoHorizontal(float Value)
{
	if (inputEnabled)
	{
		if (Value > 0)
			ordem_direcao.AddUnique(DirecaoChute::Direita);
		else if (Value < 0)
			ordem_direcao.AddUnique(DirecaoChute::Esquerda);
		else
		{
			if (ordem_direcao.Num() > 0)
			{
				ordem_direcao.Remove(DirecaoChute::Esquerda);
				ordem_direcao.Remove(DirecaoChute::Direita);
			}
		}
		MoveRight(Value);
	}
}

void APlayerCharacter::DirecaoVertical(float Value)
{
	if (inputEnabled)
	{
		if (Value > 0)
			ordem_direcao.AddUnique(DirecaoChute::Cima);
		else if (Value < 0)
			ordem_direcao.AddUnique(DirecaoChute::Baixo);
		else
		{
			if (ordem_direcao.Num() > 0)
			{
				ordem_direcao.Remove(DirecaoChute::Baixo);
				ordem_direcao.Remove(DirecaoChute::Cima);
			}
		}
	}
}

void APlayerCharacter::PlayerSlide(float Value)
{
	if (Value > 0 && !IsMoveInputIgnored() && inputEnabled)
		slideAction();
}

void APlayerCharacter::chutaPressed()
{
	if (inputEnabled)
	{
		// forca_chute = 0.0f;
		GetPlayerState<APlayerCharacterState>()->SetForcaChute(0.0f);
		GetWorldTimerManager().SetTimer(chargeHandle, this, &APlayerCharacter::charge, 0.005f, true);
	}
}

void APlayerCharacter::chutaReleased()
{
	if (inputEnabled)
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
}

void APlayerCharacter::colocadoPressed()
{
	// forca_chute = 0.0f;
	GetPlayerState<APlayerCharacterState>()->SetForcaChute(0.0f);
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
	// forca_chute += 0.02;
	float f_chute(GetPlayerState<APlayerCharacterState>()->GetForcaChute());
	f_chute += 0.02;
	if (f_chute > 1)
	{
		f_chute = 1.0f;
		// chargeTimeOut();
		// Chuta();
	}
	GetPlayerState<APlayerCharacterState>()->SetForcaChute(f_chute);
}

void APlayerCharacter::chargeTimeOut()
{
	GetWorldTimerManager().ClearTimer(chargeHandle);
}

void APlayerCharacter::pause()
{
	if (pausable && inputEnabled)
	{
		if (UGameplayStatics::IsGamePaused(GetWorld()))
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			if (pauseWidget)
				pauseWidget->RemoveFromParent();
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (IsValid(PC))
				PC->SetShowMouseCursor(false);
		}
		else
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			if (pauseWidget)
				pauseWidget->AddToViewport();
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (IsValid(PC))
				PC->SetShowMouseCursor(true);
		}
	}
}

void APlayerCharacter::SetUnpausable()
{
	pausable = false;
}