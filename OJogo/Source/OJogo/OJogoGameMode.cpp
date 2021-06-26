// Copyright Epic Games, Inc. All Rights Reserved.
#include "OJogoGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"

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
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("JogosGameState nao encontrado")));
	}

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (FutebasGI)
	{
		b = FutebasGI->team1.sigla;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("FutebasGI nao encontrado")));
	}

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
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Bot nao spawnado")));
	}
}

void AOJogoGameMode::setBotProprioGol(int32 golIndex)
{
	if (UAIBlueprintHelperLibrary::GetBlackboard(botIA))
		UAIBlueprintHelperLibrary::GetBlackboard(botIA)->SetValueAsInt(FName("proprioGol"), golIndex);
	else
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Bot sem blackboard")));
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
			offsets.Add(FVector(-1400.0f, 0.0f, 0.0f));
		}
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABola::StaticClass(), FoundActors);
	if (FoundActors.Num() == 1)
	{
		UPrimitiveComponent* Sphere = Cast<UPrimitiveComponent>(FoundActors[0]->GetRootComponent());
		if (Sphere)
		{
			Sphere->AddImpulse(Sphere->GetPhysicsLinearVelocity() * Sphere->GetMass() * (-1.0f));
			Sphere->SetPhysicsAngularVelocityInDegrees(FVector(0.0f));
			Sphere->SetWorldLocation(JogosGameState->posInicial, false, NULL, ETeleportType::TeleportPhysics);
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Sphere nao achada")));
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Bola nao achada")));

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	for (int32 Index = 0; Index != FoundActors.Num(); ++Index)
	{
		APlayerStart* PS = Cast<APlayerStart>(FoundActors[Index]);
		if (PS)
		{
			FString tag = PS->PlayerStartTag.ToString();
			int32 indicePS = FCString::Atoi(*tag);
			FVector NewLocation;
			NewLocation = offsets[indicePS] + FoundActors[Index]->GetActorLocation();
			arrayJogadores[indicePS]->SetActorLocation(NewLocation, false, NULL, ETeleportType::TeleportPhysics);
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Player Start %d nao achado"), Index));
	}
}

void AOJogoGameMode::antesDoComeco()
{
	paralisaMovimentacao(true);
	
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::antesDoComecoTimedOut, JogosGameState->tempoParadoAntesInicioPartida, false);
}

void AOJogoGameMode::antesDoComecoTimedOut()
{
	paralisaMovimentacao(false);

	if (!JogosGameState->penalidades)
		comecaJogo();
	else
		GetWorldTimerManager().SetTimer(JogosGameState->tempo1, this, &AOJogoGameMode::atualizaContagem, 10.0f, false);
}

void AOJogoGameMode::comecaJogo()
{
	GetWorldTimerManager().SetTimer(JogosGameState->tempo1, this, &AOJogoGameMode::maisAcrescimos, 5.0f, false);
	JogosGameState->tempoRegulamentar = true;
	JogosGameState->bolaEmJogo = true;
}

void AOJogoGameMode::maisAcrescimos()
{
	float timeAcrescimos = JogosGameState->acrescimos.GetSeconds() + 0.1f;
	GetWorldTimerManager().SetTimer(JogosGameState->tempo2, this, &AOJogoGameMode::terminaTempo, timeAcrescimos, false);
	JogosGameState->tempoRegulamentar = false;
}

void AOJogoGameMode::terminaTempo()
{
	JogosGameState->bolaEmJogo = false;
	if (JogosGameState->tempo1Ou2 == 1)
	{
		JogosGameState->tempo1Ou2 = 2;
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::terminaTempoTimedOut, JogosGameState->tempoParadoAntesInicioPartida, false);
	}
	else
	{
		JogosGameState->tempo1Ou2 = 3;
		fimDePapo();
	}
}

void AOJogoGameMode::terminaTempoTimedOut()
{
	JogosGameState->tempoRegulamentar = true;
	JogosGameState->acrescimos = FTimespan(0, 0, 0);
	JogosGameState->posIndex.Swap(0, 1);
	arrayJogadores.Swap(0, 1);
	trocaTimes();
	setBotProprioGol(JogosGameState->posIndex[1]);
	reiniciaPartida(true, false);
	antesDoComeco();
}

void AOJogoGameMode::fimDePapo()
{
	paralisaMovimentacao(true);

	if (JogosGameState->golsTimeDir == JogosGameState->golsTimeEsq)
	{
		vezTimeEsquerdo = JogosGameState->timeDireitoPrimeiro_pen;
		JogosGameState->tempo1Ou2 = 1;

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::penalidadesMaximas, JogosGameState->tempoParadoAntesInicioPartida, false);
	}
}

void AOJogoGameMode::trocaTimes()
{
	int32 aux;
	aux = JogosGameState->golsTimeEsq;
	JogosGameState->golsTimeEsq = JogosGameState->golsTimeDir;
	JogosGameState->golsTimeDir = aux;

	FTeamData auxTeam;
	auxTeam = FutebasGI->team1;
	FutebasGI->team1 = FutebasGI->team2;
	FutebasGI->team2 = auxTeam;
}

void AOJogoGameMode::golEsquerdo()
{
	if (JogosGameState->bolaEmJogo == true)
	{
		JogosGameState->bolaEmJogo = false;
		JogosGameState->golsTimeDir += 1;
		setBotGols(JogosGameState->golsTimeEsq, JogosGameState->golsTimeDir);
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::golEsquerdoTimedOut, JogosGameState->tempoParado, false);
	}
	else if (JogosGameState->penalidades && JogosGameState->golEsquerdoAtivado_pen)
	{
		JogosGameState->penalidades = false;
		JogosGameState->golsTimeDir_pen.Add(1);
	}
}

void AOJogoGameMode::golEsquerdoTimedOut()
{
	reiniciaPartida(false, true);
	if (JogosGameState->tempoRegulamentar)
		JogosGameState->acrescimos += FTimespan(0, 0, JogosGameState->tempoParado);
	JogosGameState->bolaEmJogo = true;
}

void AOJogoGameMode::golDireito()
{
	if (JogosGameState->bolaEmJogo == true)
	{
		JogosGameState->bolaEmJogo = false;
		JogosGameState->golsTimeEsq += 1;
		setBotGols(JogosGameState->golsTimeEsq, JogosGameState->golsTimeDir);
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::golDireitoTimedOut, JogosGameState->tempoParado, false);
	}
	else if (JogosGameState->penalidades && JogosGameState->golDireitoAtivado_pen)
	{
		JogosGameState->penalidades = false;
		JogosGameState->golsTimeEsq_pen.Add(1);
	}
}

void AOJogoGameMode::golDireitoTimedOut()
{
	reiniciaPartida(false, false);
	if (JogosGameState->tempoRegulamentar)
		JogosGameState->acrescimos += FTimespan(0, 0, JogosGameState->tempoParado);
	JogosGameState->bolaEmJogo = true;
}

void AOJogoGameMode::escanteio(AActor* pos)
{
	posicao = pos;
	if (JogosGameState->bolaEmJogo)
	{
		JogosGameState->bolaEmJogo = false;
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::escanteioTimedOut, JogosGameState->tempoParadoEscanteio, false);
	}
}

void AOJogoGameMode::escanteioTimedOut()
{
	JogosGameState->bolaEmJogo = true;
	if (JogosGameState->tempoRegulamentar)
		JogosGameState->acrescimos += FTimespan(0, 0, JogosGameState->tempoParadoEscanteio);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABola::StaticClass(), FoundActors);
	if (FoundActors.Num() == 1)
	{
		UPrimitiveComponent* Sphere = Cast<UPrimitiveComponent>(FoundActors[0]->GetRootComponent());
		if (Sphere)
		{
			Sphere->AddImpulse(Sphere->GetPhysicsLinearVelocity() * Sphere->GetMass() * (-1.0f));
			Sphere->SetPhysicsAngularVelocityInDegrees(FVector(0.0f));
			Sphere->SetWorldLocation(posicao->GetActorLocation(), false, NULL, ETeleportType::TeleportPhysics);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Sphere nao achada no escanteio")));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Bola nao achada no escanteio")));
	}
}

void AOJogoGameMode::reiniciaBolaMeio()
{
	if (JogosGameState->bolaEmJogo)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABola::StaticClass(), FoundActors);
		if (FoundActors.Num() == 1)
		{
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Achou bola na explosao")));
			UPrimitiveComponent* Sphere = Cast<UPrimitiveComponent>(FoundActors[0]->GetRootComponent());
			if (Sphere)
			{
				Sphere->AddImpulse(Sphere->GetPhysicsLinearVelocity() * Sphere->GetMass() * (-1.0f));
				Sphere->SetPhysicsAngularVelocityInDegrees(FVector(0.0f));
				FVector posOld = FoundActors[0]->GetActorLocation();
				Sphere->SetWorldLocation(FVector(posOld.X, posOld.Y, posOld.Z + JogosGameState->alturaReinicio), false, NULL, ETeleportType::TeleportPhysics);
				ABola* bola = Cast<ABola>(FoundActors[0]);
				if (bola)
					bola->explode(FVector(posOld.X, 10, 0));
				else
					GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("nao deu pra explodir")));
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Sphere nao achada na explosao")));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Bola nao achada na explosao")));
	}
}

void AOJogoGameMode::penalidadesMaximas()
{
	bool acabou;
	vezTimeEsquerdo = !vezTimeEsquerdo;

	int32 golsEsq = 0, golsDir = 0;
	for (int32 Index = 0; Index < JogosGameState->golsTimeEsq_pen.Num(); ++Index)
		golsEsq += JogosGameState->golsTimeEsq_pen[Index];

	for (int32 Index = 0; Index < JogosGameState->golsTimeDir_pen.Num(); ++Index)
		golsDir += JogosGameState->golsTimeDir_pen[Index];

	FString JoinedStrEsq("Esq:"), JoinedStrDir("Dir:");
	for (auto& golzinho : JogosGameState->golsTimeEsq_pen)
	{
		JoinedStrEsq += TEXT(" ");
		JoinedStrEsq += FString::FromInt(golzinho);
	}
	for (auto& golzinho : JogosGameState->golsTimeDir_pen)
	{
		JoinedStrDir += FString::FromInt(golzinho);
		JoinedStrDir += TEXT(" ");
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, JoinedStrEsq);
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, JoinedStrDir);

	acabou = false;
	if (JogosGameState->golsTimeEsq_pen.Num() >= 5 && JogosGameState->golsTimeDir_pen.Num() >= 5)
	{
		if (JogosGameState->golsTimeEsq_pen.Num() == JogosGameState->golsTimeDir_pen.Num())
		{
			if (golsEsq - golsDir != 0)
			{
				acabou = true;
				decideVencedor( (golsEsq - golsDir < 0) ? "time dir" : "time esq");
			}
		}
	}
	else
	{
		if ( (golsEsq - golsDir) > 5 - JogosGameState->golsTimeDir_pen.Num())
		{
			acabou = true;
			decideVencedor("time esq");
		}
		if ( (golsDir - golsEsq) > 5 - JogosGameState->golsTimeEsq_pen.Num())
		{
			acabou = true;
			decideVencedor("time dir");
		}
	}

	if(!acabou)
	{
		JogosGameState->penalidades = true;
		reiniciaPartida(false, vezTimeEsquerdo);
		JogosGameState->golEsquerdoAtivado_pen = !vezTimeEsquerdo;
		JogosGameState->golDireitoAtivado_pen = vezTimeEsquerdo;
		paralisaMovimentacao(false);
		antesDoComeco();
	}
}

void AOJogoGameMode::atualizaContagem()
{
	if (vezTimeEsquerdo && JogosGameState->penalidades)
		JogosGameState->golsTimeEsq_pen.Add(0);
	if (!vezTimeEsquerdo && JogosGameState->penalidades)
		JogosGameState->golsTimeDir_pen.Add(0);
	JogosGameState->penalidades = false;
	penalidadesMaximas();
}

void AOJogoGameMode::decideVencedor(FString timeVencedor)
{
	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, timeVencedor);
	paralisaMovimentacao(true);
}

void AOJogoGameMode::setBotGols(int32 golsEsq, int32 golsDir)
{
	if (JogosGameState->vsBotIA)
	{
		if (JogosGameState->posIndex[1] == 0)
			botIA->setBotGols(golsEsq, golsDir);
		else
			botIA->setBotGols(golsDir, golsEsq);
	}
}

void AOJogoGameMode::paralisaMovimentacao(bool ignora)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PC->SetIgnoreMoveInput(ignora);

	UAIBlueprintHelperLibrary::GetBlackboard(botIA)->SetValueAsBool(FName("canMove"), !ignora);
}