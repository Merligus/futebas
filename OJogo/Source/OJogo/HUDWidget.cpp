// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

	// JogosGameState = GetWorld()->GetAuthGameMode()->GetGameState<AOJogoGameState>();
	JogosGameState = GetWorld() != NULL ? GetWorld()->GetGameState<AOJogoGameState>() : NULL;
	if (!IsValid(JogosGameState))
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("JogosGameState nao encontrado")));
		UE_LOG(LogTemp, Warning, TEXT("JogosGameState nao encontrado"));
	}

	FutebasGI = GetGameInstance<UFutebasGameInstance>();
	if (!IsValid(FutebasGI))
		UE_LOG(LogTemp, Warning, TEXT("Futebas nao encontrado"));

	penaltis = false;
}

FText UHUDWidget::bindGoalsEsq()
{
	if (IsValid(JogosGameState))
		return UKismetTextLibrary::Conv_IntToText(JogosGameState->golsTimeEsq);
	else
		return UKismetTextLibrary::Conv_IntToText(0);
}

FText UHUDWidget::bindGoalsDir()
{
	if (IsValid(JogosGameState))
		return UKismetTextLibrary::Conv_IntToText(JogosGameState->golsTimeDir);
	else
		return UKismetTextLibrary::Conv_IntToText(0);
}

FText UHUDWidget::bindMinutes()
{
	if (IsValid(JogosGameState))
	{
		int32 minutos;
		int32 const_prorrogacao = JogosGameState->em_prorrogacao ? 90 : 0;
		int32 mult_prorrogacao = JogosGameState->em_prorrogacao ? 15 : 45;
		if (GetWorld()->GetTimerManager().IsTimerActive(JogosGameState->tempo1))
			minutos = const_prorrogacao + mult_prorrogacao * (JogosGameState->tempo1Ou2 - 1) + UKismetMathLibrary::FTrunc(GetWorld()->GetTimerManager().GetTimerElapsed(JogosGameState->tempo1));
		else if (GetWorld()->GetTimerManager().IsTimerActive(JogosGameState->tempo2))
			minutos = const_prorrogacao + mult_prorrogacao * (JogosGameState->tempo1Ou2) + UKismetMathLibrary::FTrunc(GetWorld()->GetTimerManager().GetTimerElapsed(JogosGameState->tempo2));
		else
			minutos = const_prorrogacao + JogosGameState->acrescimos.GetSeconds() + mult_prorrogacao * (JogosGameState->tempo1Ou2 - 1);
		return UKismetTextLibrary::Conv_IntToText(FMath::Clamp(minutos, 0, 400), false, true, 2);
	}
	else
		return UKismetTextLibrary::Conv_IntToText(0, false, true, 2);
}

FText UHUDWidget::bindSeconds()
{
	if (IsValid(JogosGameState))
	{
		int32 segundos;
		if (GetWorld()->GetTimerManager().IsTimerActive(JogosGameState->tempo1))
			segundos = UKismetMathLibrary::FTrunc(60 * UKismetMathLibrary::Fraction(GetWorld()->GetTimerManager().GetTimerElapsed(JogosGameState->tempo1)));
		else if (GetWorld()->GetTimerManager().IsTimerActive(JogosGameState->tempo2))
			segundos = UKismetMathLibrary::FTrunc(60 * UKismetMathLibrary::Fraction(GetWorld()->GetTimerManager().GetTimerElapsed(JogosGameState->tempo2)));
		else
			segundos = 0;
		return UKismetTextLibrary::Conv_IntToText(segundos, false, true, 2);
	}
	else
		return UKismetTextLibrary::Conv_IntToText(0, false, true, 2);
}

FText UHUDWidget::bindAcrescimosMinutos()
{
	if (IsValid(JogosGameState))
	{
		if (JogosGameState->tempoRegulamentar)
			return FText::FromString(FString(""));
		else
		{
			FText r = UKismetTextLibrary::Conv_IntToText(JogosGameState->acrescimos.GetSeconds(), false, true, 2);
			FString r_string = UKismetTextLibrary::Conv_TextToString(r);
			FString r1("+");
			FString r3("'");
			return UKismetTextLibrary::Conv_StringToText(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(r1, r_string), r3));
		}
	}
	else
		return FText::FromString(FString(""));
}

ESlateVisibility UHUDWidget::bindTempoNaoRegulamentarVisibilidade()
{
	if (IsValid(JogosGameState))
	{
		if (JogosGameState->tempoRegulamentar)
			return ESlateVisibility::Hidden;
		else
			return ESlateVisibility::Visible;
	}
	else
		return ESlateVisibility::Hidden;
}

FSlateBrush UHUDWidget::bindTime1Emb()
{
	if (FutebasGI)
		if (IsValid(JogosGameState->team1.flag))
			return UWidgetBlueprintLibrary::MakeBrushFromTexture(JogosGameState->team1.flag, 80, 60);
		else
			return FSlateNoResource();
	else
		return FSlateNoResource();
}

FSlateBrush UHUDWidget::bindTime2Emb()
{
	if (FutebasGI)
		if (IsValid(JogosGameState->team2.flag))
			return UWidgetBlueprintLibrary::MakeBrushFromTexture(JogosGameState->team2.flag, 80, 60);
		else
			return FSlateNoResource();
	else
		return FSlateNoResource();
}

FText UHUDWidget::bindTime1Nome()
{
	if (FutebasGI)
		return UKismetTextLibrary::Conv_StringToText(JogosGameState->team1.sigla);
	else
		return FText::FromString(FString("NAN"));
}

FText UHUDWidget::bindTime2Nome()
{
	if (FutebasGI)
		return UKismetTextLibrary::Conv_StringToText(JogosGameState->team2.sigla);
	else
		return FText::FromString(FString("NAN"));
}

FLinearColor UHUDWidget::bindTime1Cor1()
{
	if (FutebasGI)
		return JogosGameState->team1.jogador.roupa1;
	else
		return FLinearColor(FVector(0));
}

FLinearColor UHUDWidget::bindTime1Cor2()
{
	if (FutebasGI)
		return JogosGameState->team1.jogador.roupa2;
	else
		return FLinearColor(FVector(0));
}

FLinearColor UHUDWidget::bindTime2Cor1()
{
	if (FutebasGI)
		return JogosGameState->team2.jogador.roupa1;
	else
		return FLinearColor(FVector(0));
}

FLinearColor UHUDWidget::bindTime2Cor2()
{
	if (FutebasGI)
		return JogosGameState->team2.jogador.roupa2;
	else
		return FLinearColor(FVector(0));
}

ESlateVisibility UHUDWidget::tempoPenalidadesVisibilidade()
{
	if (penaltis)
		return ESlateVisibility::Visible;
	else
	{
		if (IsValid(JogosGameState))
		{
			if (JogosGameState->penalidades)
			{
				penaltis = true;
				return ESlateVisibility::Visible;
			}
			else
				return ESlateVisibility::Hidden;
		}
		else
			return ESlateVisibility::Hidden;
	}
}

ESlateVisibility UHUDWidget::tempoNormalVisibilidade()
{
	if (penaltis)
		return ESlateVisibility::Hidden;
	else
	{
		if (IsValid(JogosGameState))
		{
			if (JogosGameState->penalidades)
			{
				penaltis = true;
				return ESlateVisibility::Hidden;
			}
			else
				return ESlateVisibility::Visible;
		}
		else
			return ESlateVisibility::Hidden;
	}
}

FText UHUDWidget::bindGoalsEsqPen(FSlateBrush erro, FSlateBrush acerto, FSlateBrush indefinido)
{
	if (IsValid(JogosGameState))
	{
		int32 tamEsq;
		tamEsq = JogosGameState->golsTimeEsq_pen.Num();

		if (JogosGameState->golsTimeEsq_pen[tamEsq - 1] == 0)
			image_esq1->SetBrush(erro);
		else if (JogosGameState->golsTimeEsq_pen[tamEsq - 1] == 1)
			image_esq1->SetBrush(acerto);
		else
			image_esq1->SetBrush(indefinido);

		if (JogosGameState->golsTimeEsq_pen[tamEsq - 2] == 0)
			image_esq2->SetBrush(erro);
		else if (JogosGameState->golsTimeEsq_pen[tamEsq - 2] == 1)
			image_esq2->SetBrush(acerto);
		else
			image_esq2->SetBrush(indefinido);

		if (JogosGameState->golsTimeEsq_pen[tamEsq - 3] == 0)
			image_esq3->SetBrush(erro);
		else if (JogosGameState->golsTimeEsq_pen[tamEsq - 3] == 1)
			image_esq3->SetBrush(acerto);
		else
			image_esq3->SetBrush(indefinido);

		if (JogosGameState->golsTimeEsq_pen[tamEsq - 4] == 0)
			image_esq4->SetBrush(erro);
		else if (JogosGameState->golsTimeEsq_pen[tamEsq - 4] == 1)
			image_esq4->SetBrush(acerto);
		else
			image_esq4->SetBrush(indefinido);

		if (JogosGameState->golsTimeEsq_pen[tamEsq - 5] == 0)
			image_esq5->SetBrush(erro);
		else if (JogosGameState->golsTimeEsq_pen[tamEsq - 5] == 1)
			image_esq5->SetBrush(acerto);
		else
			image_esq5->SetBrush(indefinido);

		return UKismetTextLibrary::Conv_IntToText(FMath::Clamp(JogosGameState->golsSomadosTimeEsq_pen, 0, 100));
	}
	else
		return UKismetTextLibrary::Conv_IntToText(0);
}

FText UHUDWidget::bindGoalsDirPen(FSlateBrush erro, FSlateBrush acerto, FSlateBrush indefinido)
{
	if (IsValid(JogosGameState))
	{
		int32 tamDir;
		tamDir = JogosGameState->golsTimeDir_pen.Num();

		if (JogosGameState->golsTimeDir_pen[tamDir - 1] == 0)
			image_dir5->SetBrush(erro);
		else if (JogosGameState->golsTimeDir_pen[tamDir - 1] == 1)
			image_dir5->SetBrush(acerto);
		else
			image_dir5->SetBrush(indefinido);

		if (JogosGameState->golsTimeDir_pen[tamDir - 2] == 0)
			image_dir4->SetBrush(erro);
		else if (JogosGameState->golsTimeDir_pen[tamDir - 2] == 1)
			image_dir4->SetBrush(acerto);
		else
			image_dir4->SetBrush(indefinido);

		if (JogosGameState->golsTimeDir_pen[tamDir - 3] == 0)
			image_dir3->SetBrush(erro);
		else if (JogosGameState->golsTimeDir_pen[tamDir - 3] == 1)
			image_dir3->SetBrush(acerto);
		else
			image_dir3->SetBrush(indefinido);

		if (JogosGameState->golsTimeDir_pen[tamDir - 4] == 0)
			image_dir2->SetBrush(erro);
		else if (JogosGameState->golsTimeDir_pen[tamDir - 4] == 1)
			image_dir2->SetBrush(acerto);
		else
			image_dir2->SetBrush(indefinido);

		if (JogosGameState->golsTimeDir_pen[tamDir - 5] == 0)
			image_dir1->SetBrush(erro);
		else if (JogosGameState->golsTimeDir_pen[tamDir - 5] == 1)
			image_dir1->SetBrush(acerto);
		else
			image_dir1->SetBrush(indefinido);

		return UKismetTextLibrary::Conv_IntToText(FMath::Clamp(JogosGameState->golsSomadosTimeDir_pen, 0, 100));
	}
	else
		return UKismetTextLibrary::Conv_IntToText(0);
}

FText UHUDWidget::bindMinutesPenalidades()
{
	if (IsValid(JogosGameState))
	{
		int32 minutos;
		if (GetWorld()->GetTimerManager().IsTimerActive(JogosGameState->tempo1))
			minutos = UKismetMathLibrary::FTrunc(GetWorld()->GetTimerManager().GetTimerRemaining(JogosGameState->tempo1));
		else
			minutos = JogosGameState->penalty_timeout;
		return UKismetTextLibrary::Conv_IntToText(FMath::Clamp(minutos, 0, 400), false, true, 2);
	}
	else
		return UKismetTextLibrary::Conv_IntToText(0, false, true, 2);
}

FText UHUDWidget::bindSecondsPenalidades()
{
	if (IsValid(JogosGameState))
	{
		int32 segundos;
		if (GetWorld()->GetTimerManager().IsTimerActive(JogosGameState->tempo1))
			segundos = UKismetMathLibrary::FTrunc(60 * UKismetMathLibrary::Fraction(GetWorld()->GetTimerManager().GetTimerRemaining(JogosGameState->tempo1)));
		else
			segundos = 0;
		return UKismetTextLibrary::Conv_IntToText(segundos, false, true, 2);
	}
	else
		return UKismetTextLibrary::Conv_IntToText(0, false, true, 2);
}