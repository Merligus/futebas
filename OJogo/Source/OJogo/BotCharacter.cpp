// Fill out your copyright notice in the Description page of Project Settings.


#include "BotCharacter.h"
#include "BotAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void ABotCharacter::BeginPlay()
{
	Super::BeginPlay();

    // UE_LOG(LogTemp, Warning, TEXT("antes possessa"));

    // ABotAIController* AIControl = GetController<ABotAIController>();

    // UE_LOG(LogTemp, Warning, TEXT("BT ok?"));
    // if (GetBehaviorTree()->BlackboardAsset)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("AI Controller ok? BBcomp ok?"));
    //     AIControl->GetBlackboardComp()->InitializeBlackboard(*(GetBehaviorTree()->BlackboardAsset));
    // }
    // else
    //     UE_LOG(LogTemp, Warning, TEXT("Sem BlackboardAsset"));
    
    // AIControl->GetBehaviorComp()->StartTree(*GetBehaviorTree());

    meus_gols = 0;
    gols_adversario = 0;
    resultado_que_precisa = 0;

	pode_cabecear->OnComponentBeginOverlap.AddDynamic(this, &ABotCharacter::cabecearBeginOverlapBot); 
	pode_cabecear->OnComponentEndOverlap.AddDynamic(this, &ABotCharacter::cabecearEndOverlapBot);

	pode_chutar->OnComponentBeginOverlap.AddDynamic(this, &ABotCharacter::chutarBeginOverlapBot); 
	pode_chutar->OnComponentEndOverlap.AddDynamic(this, &ABotCharacter::chutarEndOverlapBot);

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	for (int32 Index = 0; Index != FoundActors.Num(); ++Index)
	{
		APlayerStart* PS = Cast<APlayerStart>(FoundActors[Index]);
		if (PS)
		{
			FString tag = PS->PlayerStartTag.ToString();
			int32 indicePS = FCString::Atoi(*tag);
            FVector Location(0);
            if (indicePS == 0)
            {
                Location = FoundActors[Index]->GetActorLocation() - FVector(100, 0, 0);
                GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsVector(FName("golEsqLocation"), Location);
            }
            else
            {
                Location = FoundActors[Index]->GetActorLocation() + FVector(100, 0, 0);
                GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsVector(FName("golDirLocation"), Location);
            }
            posicoes_gols.Add(Location);
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Bot:Player Start %d nao achado"), Index));
	}

    // JogosGameState = GetWorld()->GetAuthGameMode()->GetGameState<AOJogoGameState>();
    JogosGameState = GetWorld() != NULL ? GetWorld()->GetGameState<AOJogoGameState>() : NULL;
    if (!IsValid(JogosGameState))
        GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("Bot:JogosGameState nao encontrado")));
}

void ABotCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    sense();
    
	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
    if (TravelDirection < 0.0f)
        SetActorRotation(FRotator(0.0, 180.0f, 0.0f));
    else if (TravelDirection > 0.0f)
        SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
    else if (bisMovingRight)
        SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
    else
        SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
}

void ABotCharacter::clearMoveFinish()
{
    GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsBool(FName("moveToBallFinish"), true);
    GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsBool(FName("moveToGoalFinish"), true);
}

void ABotCharacter::setOwnGoal(int32 pos_index)
{
    GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsInt(FName("proprioGol"), pos_index);
}

void ABotCharacter::setBotGols(int32 meus, int32 adversario)
{
	meus_gols = meus;
    gols_adversario = adversario;
}

void ABotCharacter::BotChuta(float forca, float angulo)
{
    // forca_chute = forca;
    GetPlayerState<APlayerCharacterState>()->SetForcaChute(forca);
    chute_angulo->SetRelativeRotation(FRotator(angulo, 0, 0));

    Super::Chuta();
}

void ABotCharacter::cabecearBeginOverlapBot(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, 
										    UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, 
										    bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		auto bola_temp = Cast<ABola>(OtherActor);
		if (IsValid(bola_temp))
        {
			if (bola_temp->GetRootComponent() == OtherComp)
            {
				canHeader = true;
                GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsBool(FName("canKick"), canHeader);
            }
        }
	}
}

void ABotCharacter::cabecearEndOverlapBot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
										  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		auto bola_temp = Cast<ABola>(OtherActor);
		if (IsValid(bola_temp))
        {
			if (bola_temp->GetRootComponent() == OtherComp)
            {
				canHeader = false;
                GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsBool(FName("canKick"), canHeader);
            }
        }
            
	}
}

void ABotCharacter::chutarBeginOverlapBot(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, 
										  UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, 
										  bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		auto bola_temp = Cast<ABola>(OtherActor);
		if (IsValid(bola_temp))
        {
			if (bola_temp->GetRootComponent() == OtherComp)
            {
				canKick = true;
                GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsBool(FName("canKick"), canKick);
            }
        }
	}
}

void ABotCharacter::chutarEndOverlapBot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
									    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		auto bola_temp = Cast<ABola>(OtherActor);
		if (IsValid(bola_temp))
        {
			if (bola_temp->GetRootComponent() == OtherComp)
            {
				canKick = false;
                GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsBool(FName("canKick"), canKick);
            }
        }
	}
}

void ABotCharacter::horizontalDistance(FVector v1, FVector v2, float& abs, float& d)
{
    d = v1.X - v2.X;
    abs = UKismetMathLibrary::Abs(d);
}

void ABotCharacter::verticalDistance(FVector v1, FVector v2, float& abs, float& d)
{
    d = v1.Z - v2.Z;
    abs = UKismetMathLibrary::Abs(d);
}

FVector ABotCharacter::futurePosBall(FVector location, FVector velocity, float time)
{
    return location + time * velocity;
}

bool ABotCharacter::areComponentsNear(FVector A_Location, FVector B_Location, float radius)
{
    return UKismetMathLibrary::Vector_IsNearlyZero(A_Location - B_Location, radius);
}

void ABotCharacter::sense()
{
    float ball_velocity_threshold(1000);
    float horizontal_abs_distance_threshold(1100);
    float time_pos_cabeceio(0.2f);
    float time_cross_planes(0.7f);
    float time_carrinho(0.8f);
    float time_head(1.0f);
    float near_radius_threshold(2);
    float save_jumps_stamina(2);
    float vertical_ball_to_head_distance_threshold(150);
    float horizontal_ball_to_head_abs_distance_threshold(100);
    FVector head_height_threshold(0, 0, 282);

    FVector player_location(Cast<AOJogoCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetActorLocation());
    
    TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABola::StaticClass(), FoundActors);
	if (FoundActors.Num() == 1)
		ball = Cast<ABola>(FoundActors[0]);
        
    bool go_to_ball1(false), go_to_ball2(false), go_to_ball3(false);
    bool go_to_ball;
    float abs1(0), d1(0), abs2(0), d2(0);
    if (IsValid(ball))
    {
        horizontalDistance(player_location, ball->GetActorLocation(), abs1, d1);
        horizontalDistance(GetActorLocation(), ball->GetActorLocation(), abs2, d2);
        go_to_ball1 = (abs2 - abs1) < horizontal_abs_distance_threshold;
        go_to_ball2 = UKismetMathLibrary::Abs(ball->GetVelocity().X) < ball_velocity_threshold;
    }
    go_to_ball3 = (meus_gols - gols_adversario) < resultado_que_precisa;
    go_to_ball = (go_to_ball1 && go_to_ball2) || go_to_ball3;
    GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsBool(FName("goToBall"), go_to_ball);
    bisMovingRight = d2 < 0;

    bool jump1(false), jump2(false), jump3(false), jump4(false), jump5(false), jump6(false);
    bool jump;
    if (IsValid(ball))
    {
        FVector future_ball_cross_planes = futurePosBall(ball->GetActorLocation(), ball->GetVelocity(), time_cross_planes);
        FVector future_ball_head = futurePosBall(ball->GetActorLocation(), ball->GetVelocity(), time_head);
        float t;
        FVector intersection;
        FVector up_head_point = cabeca->GetComponentLocation() + head_height_threshold;
        FPlane plane1 = UKismetMathLibrary::MakePlaneFromPointAndNormal(up_head_point, FVector(0, 0, -1));
        FPlane plane2 = UKismetMathLibrary::MakePlaneFromPointAndNormal(up_head_point, FVector(-1, 0, 0));
        FPlane plane3 = UKismetMathLibrary::MakePlaneFromPointAndNormal(cabeca->GetComponentLocation(), FVector(0, 0, 1));
        jump1 = UKismetMathLibrary::LinePlaneIntersection(ball->GetActorLocation(), future_ball_cross_planes, plane1, t, intersection);
        jump2 = UKismetMathLibrary::LinePlaneIntersection(ball->GetActorLocation(), future_ball_cross_planes, plane2, t, intersection);
        jump3 = UKismetMathLibrary::LinePlaneIntersection(ball->GetActorLocation(), future_ball_head, plane3, t, intersection);
        jump4 = areComponentsNear(ball->GetActorLocation(), cabeca->GetComponentLocation(), near_radius_threshold * cabeca->GetScaledSphereRadius());
        verticalDistance(ball->GetActorLocation(), cabeca->GetComponentLocation(), abs1, d1);
        horizontalDistance(ball->GetActorLocation(), cabeca->GetComponentLocation(), abs2, d2);
        jump5 = d1 > vertical_ball_to_head_distance_threshold;
        jump6 = abs2 < horizontal_ball_to_head_abs_distance_threshold;
    }
    jump = (jump1 && jump2) || jump4 || (jump3 && jump5 && jump6);
    GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsBool(FName("jump"), jump);
    GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsBool(FName("cabeceia"), jump);

    if (IsValid(ball))
    {
        FVector future_ball_cabeceio = futurePosBall(ball->GetActorLocation(), ball->GetVelocity(), time_pos_cabeceio);
        GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsFloat(FName("posCabeceio"), future_ball_cabeceio.X);

        bool carrinho1(false), carrinho2(false), carrinho3(false), carrinho4(false);
        bool carrinho;

        horizontalDistance(GetActorLocation(), ball->GetActorLocation(), abs1, d1);
        int32 ind_pos_gol = GetController<ABotAIController>()->GetBlackboardComp()->GetValueAsInt(FName("proprioGol"));
        horizontalDistance(GetActorLocation(), posicoes_gols[ind_pos_gol], abs2, d2);
        carrinho1 = abs1 > horizontal_abs_distance_threshold;
        carrinho2 = abs2 > horizontal_abs_distance_threshold;
        carrinho3 = UKismetMathLibrary::Abs(ball->GetVelocity().X) < ball_velocity_threshold;
        carrinho4 = GetPlayerState<APlayerCharacterState>()->GetStamina() > (save_jumps_stamina * jumpStaminaCost + slidingStaminaCost);
        carrinho = ((carrinho1 && go_to_ball) || (carrinho2 && !go_to_ball)) && carrinho3 && carrinho4;

        FVector future_ball_carrinho = futurePosBall(ball->GetActorLocation(), ball->GetVelocity(), time_carrinho);
        float carrinhoDirecao = carrinho? future_ball_carrinho.X : 0.0f;

        if (IsValid(JogosGameState))
        {
            if (JogosGameState->bolaEmJogo)
                GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsFloat(FName("carrinhoDirecao"), carrinhoDirecao);
            else
                GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsFloat(FName("carrinhoDirecao"), 0.0f);
        }
    }

    GetController<ABotAIController>()->GetBlackboardComp()->SetValueAsBool(FName("moving"), UKismetMathLibrary::VSize(GetVelocity()) > 0);
}