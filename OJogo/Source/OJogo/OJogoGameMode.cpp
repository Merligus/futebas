// Copyright Epic Games, Inc. All Rights Reserved.
#include "OJogoGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PlayerStart.h"
#include "BotAIController.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "Camera/CameraActor.h"
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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	for (int32 player_index = 1; player_index < PlayerStarts.Num(); ++player_index)
		UGameplayStatics::CreatePlayer(GetWorld(), player_index);
	for (int32 player_index = 0; player_index < PlayerStarts.Num(); ++player_index)
	{
		if (player_index == 0 || !FutebasGI->vs_bot)
		{
			APlayerCharacter* player = GetWorld()->SpawnActorDeferred<APlayerCharacter>(playerClass.Get(), FTransform());
			if (player)
			{
				player->SetIndexController(player_index);
				UGameplayStatics::FinishSpawningActor(player, FTransform());
			}
			else
				UE_LOG(LogTemp, Warning, TEXT("player %d not spawned"), player_index);
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), player_index);
			APawn* pawn = Cast<APawn>(player);
			if (IsValid(pawn))
				PC->Possess(pawn);
		}
	}

	// game instance
	// FutebasGI = Cast<UFutebasGameInstance>(GetGameInstance());
	// GetGameInstance()
	// game state
	// GetGameState<AOJogoGameState>();
	// GetWorld()->GetAuthGameMode()->GetGameState<AOJogoGameState>()->vsBotIA = true;
}

// Called every frame
void AOJogoGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOJogoGameMode::beginGame()
{	
	if (FutebasGI)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABola::StaticClass(), FoundActors);
		if (FoundActors.Num() == 1)
			BolaActor = Cast<ABola>(FoundActors[0]);
		else
			UE_LOG(LogTemp, Warning, TEXT("FoundActors pra bola != 1"));

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundActors);
		for (int32 player_index = 0; player_index < FoundActors.Num(); ++player_index)
		{
			APlayerCharacter* player = Cast<APlayerCharacter>(FoundActors[player_index]);
			player->setHabilidades((player->GetIndexController() < PlayerStarts.Num()/2)? FutebasGI->team1.habilidades : FutebasGI->team2.habilidades);
			player->setJogador((player->GetIndexController() < PlayerStarts.Num()/2)? FutebasGI->team1.jogador : FutebasGI->team2.jogador);
			arrayJogadores.Add(player);
		}

		if (FutebasGI->vs_bot)
		{
			for (int32 Index = 0; Index < PlayerStarts.Num(); ++Index)
			{
				APlayerStart* PS = Cast<APlayerStart>(PlayerStarts[Index]);
				if (IsValid(PS))
				{
					FString tag = PS->PlayerStartTag.ToString();
					int32 indicePS = FCString::Atoi(*tag);
					if (indicePS == PlayerStarts.Num()-1)
					{
						botIA = Cast<AOJogoCharacter>(GetWorld()->SpawnActorAbsolute(botClass.Get(), PS->GetActorTransform()));
						if(IsValid(botIA))
						{
							botIA->SpawnDefaultController();
							botIA->setHabilidades(FutebasGI->team2.habilidades);
							botIA->setJogador(FutebasGI->team2.jogador);
							arrayJogadores.Add(botIA);
							Cast<ABotCharacter>(botIA)->setOwnGoal(JogosGameState->posIndex[1]);
						}
						else
							UE_LOG(LogTemp, Warning, TEXT("Bot nao spawnado"));
					}
				}
			}
		}

		if (!FutebasGI->team1_em_casa)
		{
			JogosGameState->posIndex.Swap(0, 1);
			Cast<ABotCharacter>(botIA)->setOwnGoal(JogosGameState->posIndex[1]);
			trocaTimes();
		}

		if (JogosGameState->posIndex[0] == 1)
			arrayJogadores.Swap(0, 1);
		reiniciaPartida(true, false);
		antesDoComeco();
	}
}

void AOJogoGameMode::fazSomApito(int32 modo)
{
	if (modo == 0)
	{
		if (som_apito_inicio)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), som_apito_inicio, JogosGameState->posInicial);
		else
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("som apito inicio nao encontrado")));
	}
	else if (modo == 1)
	{
		if (som_apito_fim)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), som_apito_fim, JogosGameState->posInicial);
		else
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("som apito fim nao encontrado")));
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
			offsets.Add(FVector(-1400.0f, 0.0f, 0.0f));
		}
	}
	
	if (IsValid(BolaActor))
	{
		UPrimitiveComponent* Sphere = Cast<UPrimitiveComponent>(BolaActor->GetRootComponent());
		if (Sphere)
		{
			Sphere->AddImpulse(Sphere->GetPhysicsLinearVelocity() * Sphere->GetMass() * (-1.0f));
			Sphere->SetPhysicsAngularVelocityInDegrees(FVector(0.0f));
			Sphere->SetWorldLocation(JogosGameState->posInicial, false, NULL, ETeleportType::TeleportPhysics);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Sphere nao achada"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Bola nao achada"));

	for (int32 Index = 0; Index != PlayerStarts.Num(); ++Index)
	{
		APlayerStart* PS = Cast<APlayerStart>(PlayerStarts[Index]);
		if (PS)
		{
			FString tag = PS->PlayerStartTag.ToString();
			int32 indicePS = FCString::Atoi(*tag);
			FTransform NewLocation(PS->GetActorTransform());
			NewLocation.SetLocation(offsets[indicePS] + PlayerStarts[Index]->GetActorLocation());
			arrayJogadores[indicePS]->SetActorTransform(NewLocation, false, NULL, ETeleportType::TeleportPhysics);
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Player Start %d nao achado"), Index));
	}
}

void AOJogoGameMode::antesDoComeco()
{
	// cutscene
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		ALevelSequenceActor* actor = Cast<ALevelSequenceActor>(FoundActors[0]);
		actor->SequencePlayer->Play();
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("abertura not valid"));

	paralisaMovimentacao(true);
	
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::antesDoComecoTimedOut, JogosGameState->tempoParadoAntesInicioPartida, false);
}

void AOJogoGameMode::antesDoComecoTimedOut()
{
	paralisaMovimentacao(false);
	fazSomApito(0);

	if (!JogosGameState->penalidades)
		comecaJogo();
	else
		GetWorldTimerManager().SetTimer(JogosGameState->tempo1, this, &AOJogoGameMode::penaltyTimedOut, JogosGameState->penalty_timeout, false);
}

void AOJogoGameMode::comecaJogo()
{
	if (JogosGameState->em_prorrogacao)
		GetWorldTimerManager().SetTimer(JogosGameState->tempo1, this, &AOJogoGameMode::maisAcrescimos, 15.0f, false);
	else
		GetWorldTimerManager().SetTimer(JogosGameState->tempo1, this, &AOJogoGameMode::maisAcrescimos, 45.0f, false);
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
		fazSomApito(0);
		JogosGameState->tempo1Ou2 = 2;
		GetWorldTimerManager().ClearTimer(delayTimedOut);
		paralisaMovimentacao(true);
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
	paralisaMovimentacao(false);
	JogosGameState->tempoRegulamentar = true;
	JogosGameState->acrescimos = FTimespan(0, 0, 0);
	JogosGameState->posIndex.Swap(0, 1);
	arrayJogadores.Swap(0, 1);
	trocaTimes();
	if (IsValid(botIA) && FutebasGI->vs_bot)
		Cast<ABotCharacter>(botIA)->setOwnGoal(JogosGameState->posIndex[1]);
	reiniciaPartida(true, false);
	antesDoComeco();
}

void AOJogoGameMode::fimDePapo()
{
	paralisaMovimentacao(true);

	if (JogosGameState->golsTimeDir == JogosGameState->golsTimeEsq && FutebasGI->desempate_por_penaltis)
	{
		fazSomApito(1);
		if (!JogosGameState->em_prorrogacao)
		{
			JogosGameState->em_prorrogacao = true;
			JogosGameState->tempo1Ou2 = 1;
			GetWorldTimerManager().ClearTimer(delayTimedOut);
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::terminaTempoTimedOut, JogosGameState->tempoParadoAntesInicioPartida, false);
		}
		else
		{
			JogosGameState->penalidades = true;
			JogosGameState->tempo1Ou2 = 1;
			vezTimeEsquerdo = JogosGameState->timeDireitoPrimeiro_pen;
			JogosGameState->tempoRegulamentar = true;
			setBotGols(0, 100);
			GetWorldTimerManager().ClearTimer(delayTimedOut);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::penalidadesMaximas, JogosGameState->tempoParadoAntesInicioPartida, false);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(delayTimedOut);
		decideVencedor();
	}
}

void AOJogoGameMode::trocaTimes()
{
	int32 aux;
	aux = JogosGameState->golsTimeEsq;
	JogosGameState->golsTimeEsq = JogosGameState->golsTimeDir;
	JogosGameState->golsTimeDir = aux;

	aux = JogosGameState->golsSomadosTimeEsq_pen;
	JogosGameState->golsSomadosTimeEsq_pen = JogosGameState->golsSomadosTimeDir_pen;
	JogosGameState->golsSomadosTimeDir_pen = aux;

	TArray<int32> auxArray;
	auxArray = JogosGameState->golsTimeEsq_pen;
	JogosGameState->golsTimeEsq_pen = JogosGameState->golsTimeDir_pen;
	JogosGameState->golsTimeDir_pen = auxArray;

	FTeamData auxTeam;
	auxTeam = FutebasGI->team1;
	FutebasGI->team1 = FutebasGI->team2;
	FutebasGI->team2 = auxTeam;
}

bool AOJogoGameMode::golEsquerdo()
{
	bool bola_em_jogo = false;
	if (JogosGameState->bolaEmJogo == true)
	{
		bola_em_jogo = true;
		JogosGameState->bolaEmJogo = false;
		paralisaMovimentacao(true);
		JogosGameState->golsTimeDir += 1;
		setBotGols(JogosGameState->golsTimeEsq, JogosGameState->golsTimeDir);
		GetWorldTimerManager().SetTimer(delayTimedOut, this, &AOJogoGameMode::golEsquerdoTimedOut, JogosGameState->tempoParado, false);
	}
	else if (JogosGameState->penalidades && GetWorldTimerManager().IsTimerActive(JogosGameState->tempo1))
	{
		GetWorldTimerManager().PauseTimer(JogosGameState->tempo1);
		paralisaMovimentacao(true);
		if (JogosGameState->golEsquerdoAtivado_pen)
		{
			bola_em_jogo = true;
			golTimeDir = true;
		}
		atualizaContagem();
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::penalidadesMaximas, JogosGameState->tempoParado, false);
	}
	return bola_em_jogo;
}

void AOJogoGameMode::golEsquerdoTimedOut()
{
	reiniciaPartida(true, false);
	paralisaMovimentacao(false);
	if (JogosGameState->tempoRegulamentar)
		JogosGameState->acrescimos += FTimespan(0, 0, JogosGameState->tempoParado);
	JogosGameState->bolaEmJogo = true;
}

bool AOJogoGameMode::golDireito()
{
	bool bola_em_jogo = false;
	if (JogosGameState->bolaEmJogo == true)
	{
		bola_em_jogo = true;
		JogosGameState->bolaEmJogo = false;
		paralisaMovimentacao(true);
		JogosGameState->golsTimeEsq += 1;
		setBotGols(JogosGameState->golsTimeEsq, JogosGameState->golsTimeDir);
		GetWorldTimerManager().SetTimer(delayTimedOut, this, &AOJogoGameMode::golDireitoTimedOut, JogosGameState->tempoParado, false);
	}
	else if (JogosGameState->penalidades && GetWorldTimerManager().IsTimerActive(JogosGameState->tempo1))
	{
		GetWorldTimerManager().PauseTimer(JogosGameState->tempo1);
		paralisaMovimentacao(true);
		if (JogosGameState->golDireitoAtivado_pen)
		{
			bola_em_jogo = true;
			golTimeEsq = true;
		}
		atualizaContagem();
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::penalidadesMaximas, JogosGameState->tempoParado, false);
	}
	return bola_em_jogo;
}

void AOJogoGameMode::golDireitoTimedOut()
{
	reiniciaPartida(true, false);
	paralisaMovimentacao(false);
	if (JogosGameState->tempoRegulamentar)
		JogosGameState->acrescimos += FTimespan(0, 0, JogosGameState->tempoParado);
	JogosGameState->bolaEmJogo = true;
}

bool AOJogoGameMode::escanteio(AActor* pos)
{
	bool bola_em_jogo = JogosGameState->bolaEmJogo;
	posicao = pos;
	if (JogosGameState->bolaEmJogo)
	{
		JogosGameState->bolaEmJogo = false;
		GetWorldTimerManager().SetTimer(delayTimedOut, this, &AOJogoGameMode::escanteioTimedOut, JogosGameState->tempoParadoEscanteio, false);
	}
	else if (JogosGameState->penalidades && GetWorldTimerManager().IsTimerActive(JogosGameState->tempo1))
	{
		GetWorldTimerManager().PauseTimer(JogosGameState->tempo1);
		paralisaMovimentacao(true);
		atualizaContagem();
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoGameMode::penalidadesMaximas, JogosGameState->tempoParado, false);
	}
	return bola_em_jogo;
}

void AOJogoGameMode::escanteioTimedOut()
{
	JogosGameState->bolaEmJogo = true;
	if (JogosGameState->tempoRegulamentar)
		JogosGameState->acrescimos += FTimespan(0, 0, JogosGameState->tempoParadoEscanteio);

	if (IsValid(BolaActor))
	{
		UPrimitiveComponent* Sphere = Cast<UPrimitiveComponent>(BolaActor->GetRootComponent());
		if (Sphere)
		{
			Sphere->AddImpulse(Sphere->GetPhysicsLinearVelocity() * Sphere->GetMass() * (-1.0f));
			Sphere->SetPhysicsAngularVelocityInDegrees(FVector(0.0f));
			Sphere->SetWorldLocation(posicao->GetActorLocation(), false, NULL, ETeleportType::TeleportPhysics);
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Sphere nao achada no escanteio")));
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Bola nao achada no escanteio")));
}

void AOJogoGameMode::reiniciaBolaMeio()
{
	if (JogosGameState->bolaEmJogo)
	{
		if (IsValid(BolaActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Achou bola na explosao")));
			UPrimitiveComponent* Sphere = Cast<UPrimitiveComponent>(BolaActor->GetRootComponent());
			if (Sphere)
			{
				Sphere->AddImpulse(Sphere->GetPhysicsLinearVelocity() * Sphere->GetMass() * (-1.0f));
				Sphere->SetPhysicsAngularVelocityInDegrees(FVector(0.0f));
				FVector posOld = BolaActor->GetActorLocation();
				Sphere->SetWorldLocation(FVector(posOld.X, posOld.Y, posOld.Z + JogosGameState->alturaReinicio), false, NULL, ETeleportType::TeleportPhysics);
				if (BolaActor)
					BolaActor->explode(FVector(posOld.X, 10, 0));
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

	// debug
	// FString JoinedStrEsq("Esq:"), JoinedStrDir("Dir:");
	// for (auto& golzinho : JogosGameState->golsTimeEsq_pen)
	// {
	// 	JoinedStrEsq += TEXT(" ");
	// 	JoinedStrEsq += FString::FromInt(golzinho);
	// }
	// for (auto& golzinho : JogosGameState->golsTimeDir_pen)
	// {
	// 	JoinedStrDir += FString::FromInt(golzinho);
	// 	JoinedStrDir += TEXT(" ");
	// }
	
	// GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, JoinedStrEsq);
	// GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, JoinedStrDir);
	
	vezTimeEsquerdo = !vezTimeEsquerdo;
	acabou = false;
	if (JogosGameState->golsTimeEsq_pen.Num() > 5 && JogosGameState->golsTimeDir_pen.Num() > 5)
	{
		if (faltamEsq_pen == faltamDir_pen)
		{
			if (JogosGameState->golsSomadosTimeEsq_pen - JogosGameState->golsSomadosTimeDir_pen != 0)
			{
				acabou = true;
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("no alternado terminou")));
				decideVencedor();
			}
		}
	}
	else
	{
		if ( (JogosGameState->golsSomadosTimeEsq_pen - JogosGameState->golsSomadosTimeDir_pen) > faltamDir_pen)
		{
			acabou = true;
			decideVencedor();
		}
		else if ( (JogosGameState->golsSomadosTimeDir_pen - JogosGameState->golsSomadosTimeEsq_pen) > faltamEsq_pen)
		{
			acabou = true;
			decideVencedor();
		}
	}

	if(!acabou)
	{
		reiniciaPartida(false, vezTimeEsquerdo);
		golTimeEsq = false;
		golTimeDir = false;
		JogosGameState->golEsquerdoAtivado_pen = !vezTimeEsquerdo;
		JogosGameState->golDireitoAtivado_pen = vezTimeEsquerdo;
		paralisaMovimentacao(false);
		antesDoComeco();
	}
}

void AOJogoGameMode::atualizaContagem()
{
	if (vezTimeEsquerdo)
	{
		if (!golTimeEsq)
			JogosGameState->golsTimeEsq_pen = atualizaPenalidades(JogosGameState->golsTimeEsq_pen, 0);
		else
			JogosGameState->golsTimeEsq_pen = atualizaPenalidades(JogosGameState->golsTimeEsq_pen, 1);
	}
	if (!vezTimeEsquerdo)
	{
		if (!golTimeDir)
			JogosGameState->golsTimeDir_pen = atualizaPenalidades(JogosGameState->golsTimeDir_pen, 0);
		else
			JogosGameState->golsTimeDir_pen = atualizaPenalidades(JogosGameState->golsTimeDir_pen, 1);
	}

	JogosGameState->golsSomadosTimeEsq_pen = 0;
	faltamEsq_pen = 0;
	for (int32 Index = 0; Index < JogosGameState->golsTimeEsq_pen.Num(); ++Index)
	{
		if (JogosGameState->golsTimeEsq_pen[Index] != 2)
			JogosGameState->golsSomadosTimeEsq_pen += JogosGameState->golsTimeEsq_pen[Index];
		else
			faltamEsq_pen++;
	}
	
	JogosGameState->golsSomadosTimeDir_pen = 0;
	faltamDir_pen = 0;
	for (int32 Index = 0; Index < JogosGameState->golsTimeDir_pen.Num(); ++Index)
	{
		if (JogosGameState->golsTimeDir_pen[Index] != 2)
			JogosGameState->golsSomadosTimeDir_pen += JogosGameState->golsTimeDir_pen[Index];
		else
			faltamDir_pen++;
	}
}

TArray<int32> AOJogoGameMode::atualizaPenalidades(TArray<int32> array_pen, int32 pen)
{
	int32 i;
	if (array_pen[array_pen.Num() - 1] != 2)
	{
		for (i = 0; i < 5; ++i)
		{
			JogosGameState->golsTimeEsq_pen.Add(2);
			JogosGameState->golsTimeDir_pen.Add(2);
			array_pen.Add(2);
		}
	}
	i = array_pen.Num() - 1;
	while (i >= 0)
	{
		if (array_pen[i] != 2)
			break;
		--i;
	}
	array_pen[i + 1] = pen;
	return array_pen;
}

void AOJogoGameMode::penaltyTimedOut()
{
	atualizaContagem();
	penalidadesMaximas();
}

void AOJogoGameMode::decideVencedor()
{
	paralisaMovimentacao(true);
	fazSomApito(1);
	
	trocaTimes();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAmbientSound::StaticClass(), FoundActors);
	for (int32 index = 0; index < FoundActors.Num(); ++index)
	{
		AAmbientSound* AS = Cast<AAmbientSound>(FoundActors[index]);
		if (IsValid(AS))
		{
			if (AS->Tags.Num() > 0)
			{
				FString tag = AS->Tags[0].ToString();
				if (tag.Equals(FString(TEXT("goal"))))
					comemorando_titulo = AS;
			}
		}
	}
	
	if (comemorando_titulo)
	{
		comemorando_titulo->FadeIn(1.0f, 1.0f);
		FTimerHandle UnusedHandle1;
		GetWorldTimerManager().SetTimer(UnusedHandle1, this, &AOJogoGameMode::comemoracaoTimedOut, 2, false);
	}

	if (FutebasGI)
	{
		int32 teams_ind((int32)FutebasGI->current_teams_set);
		if (FutebasGI->current_game_mode == GameMode::CopaMundo)
		{
			if (FutebasGI->GetCopa(teams_ind)->fase_atual == 4)
			{
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFireworkActor::StaticClass(), FoundActors);
				for (int32 index = 0; index < FoundActors.Num(); ++index)
				{
					AFireworkActor* firework = Cast<AFireworkActor>(FoundActors[index]);
					firework->Fire();
				}
				
				FTimerHandle UnusedHandle2;
				GetWorldTimerManager().SetTimer(UnusedHandle2, this, &AOJogoGameMode::openAfterMatch, 10, false);
			}
			else
				openAfterMatch();
		}
		else
			openAfterMatch();
	}
}

void AOJogoGameMode::comemoracaoTimedOut()
{
	comemorando_titulo->FadeOut(1.0f, 0.0f);
}

void AOJogoGameMode::openAfterMatch()
{
	if (IsValid(widgetClass))
	{
		matchResultWidget = Cast<UAfterMatchWidget>(CreateWidget(GetWorld(), widgetClass));

		if (matchResultWidget)
			matchResultWidget->AddToViewport();
	}
}

void AOJogoGameMode::setBotGols(int32 golsEsq, int32 golsDir)
{
	if (IsValid(botIA) && FutebasGI->vs_bot)
	{
		if (JogosGameState->posIndex[1] == 0)
			Cast<ABotCharacter>(botIA)->setBotGols(golsEsq, golsDir);
		else
			Cast<ABotCharacter>(botIA)->setBotGols(golsDir, golsEsq);
	}
}

void AOJogoGameMode::paralisaMovimentacao(bool ignora)
{
	for (int32 Index = 0; Index != PlayerStarts.Num(); ++Index)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), Index);
		if (IsValid(PC))
			if (PC->IsMoveInputIgnored() != ignora)
				PC->SetIgnoreMoveInput(ignora);
	}

	if (IsValid(botIA) && FutebasGI->vs_bot)
		UAIBlueprintHelperLibrary::GetBlackboard(botIA)->SetValueAsBool(FName("canMove"), !ignora);
}