// Copyright Epic Games, Inc. All Rights Reserved.
#include "OJogoGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

AOJogoGameMode::AOJogoGameMode()
{
	// Set default pawn class to our character
	// DefaultPawnClass = AOJogoCharacter::StaticClass();	
}

void AOJogoGameMode::BeginPlay()
{
	int32 a;
	FString b("NAN");

	Super::BeginPlay();

	JogosGameState = GetWorld()->GetAuthGameMode()->GetGameState<AOJogoGameState>();
	if (JogosGameState)
	{
		a = JogosGameState->tempo1Ou2;
	}
	else
	{
		a = -1;
	}

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (FutebasGI)
	{
		b = FutebasGI->team1.sigla;
	}
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Begin Play: %d"), a));
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, b);

	// game instance
	// FutebasGI = Cast<UFutebasGameInstance>(GetGameInstance());
	// GetGameInstance()
	// game state
	// GetGameState<AOJogoGameState>();
	// GetWorld()->GetAuthGameMode()->GetGameState<AOJogoGameState>()->vsBotIA = true;
}

void AOJogoGameMode::beginGame()
{
	player1 = Cast<AOJogoCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (FutebasGI)
	{
		player1->setHabilidades(FutebasGI->team1.habilidades);
		player1->setJogador(FutebasGI->team1.jogador);

		botIA = GetWorld()->SpawnActor<AOJogoCharacter>(JogosGameState->botIA_c, FVector(150, -20, 0), FRotator(0));
		if(botIA)
		{
			botIA->SpawnDefaultController();
			botIA->setHabilidades(FutebasGI->team2.habilidades);
			botIA->setJogador(FutebasGI->team2.jogador);
			arrayJogadores.Add(player1);
			arrayJogadores.Add(botIA);
			if (JogosGameState->posIndex[0] == 1)
				arrayJogadores.Swap(0, 1);
			setBotProprioGol(JogosGameState->posIndex[1]);
			reiniciaPartida(true, false);
			antesDoComeco();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Bot nao spawnado")));
		}
	}
}

void AOJogoGameMode::setBotProprioGol(int32 golIndex)
{
	if (UAIBlueprintHelperLibrary::GetBlackboard(botIA))
	{
		UAIBlueprintHelperLibrary::GetBlackboard(botIA)->SetValueAsInt(FName("proprioGol"), golIndex);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Bot sem blackboard")));
	}
}

void AOJogoGameMode::reiniciaPartida(bool neutro, bool favoravelEsq)
{
	TArray<FVector> offsets;
	if (neutro)
	{
		offsets.Add(FVector(0.0f));
		offsets.Add(FVector(0.0f));
	}
	else
	{
		if (favoravelEsq)
		{
			offsets.Add(FVector(1400.0f, 0.0f, 0.0f));
			offsets.Add(FVector(0.0f));
		}
		else
		{
			offsets.Add(FVector(0.0f));
			offsets.Add(FVector(1400.0f, 0.0f, 0.0f));
		}
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), JogosGameState->Bola_c, FoundActors);
	if (FoundActors[0])
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Achou bola")));
		UPrimitiveComponent* Sphere = Cast<UPrimitiveComponent>(FoundActors[0]->GetRootComponent());
		if (Sphere)
		{
			Sphere->AddImpulse(Sphere->GetPhysicsLinearVelocity() * Sphere->GetMass() * (-1.0f));
			Sphere->SetPhysicsAngularVelocity(FVector(0.0f));
			Sphere->SetWorldLocation(JogosGameState->posInicial);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Sphere nao achada")));
		}
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Bola nao achada")));
	}
}

void AOJogoGameMode::antesDoComeco()
{
	
}
