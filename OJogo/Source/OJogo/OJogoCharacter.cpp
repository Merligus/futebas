// Copyright Epic Games, Inc. All Rights Reserved.

#include "OJogoCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AOJogoCharacter

AOJogoCharacter::AOJogoCharacter(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	// CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// CameraBoom->SetupAttachment(RootComponent);
	// CameraBoom->TargetArmLength = 500.0f;
	// CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	// CameraBoom->SetUsingAbsoluteRotation(true);
	// CameraBoom->bDoCollisionTest = false;
	// CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	// CameraBoom->SetIsReplicated(true);	

	// Create an orthographic camera (no perspective) and attach it to the boom
	// SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	// SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	// SideViewCameraComponent->OrthoWidth = 2048.0f;
	// // SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// SideViewCameraComponent->SetIsReplicated(true);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	// CameraBoom->SetUsingAbsoluteRotation(true);
	// SideViewCameraComponent->bUsePawnControlRotation = false;
	// SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->MaxAcceleration = 10000.0f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 10000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 2048.0f;
	GetCharacterMovement()->AirControl = 0.8;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(false);
	GetCharacterMovement()->SetIsReplicated(true);
	// bReplicateMovement = true;
	SetReplicateMovement(true);
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	// bReplicateInstigator = true;

	maxForcaChute = 5000.0f;
	maxForcaCabeceio = 5000.0f;
	velocidade = GetCharacterMovement()->MaxWalkSpeed;
	velocidadeCarrinho = 1200.0f;
	aceleracaoCarrinho = 10000.0f;
	staminaRegen = 0.05f;
	jumpStaminaCost = 10.0f;
	slidingStaminaCost = 20.0f;

	cabelo = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("cabelo"));
	if (cabelo)
	{
		cabelo->AlwaysLoadOnClient = true;
		cabelo->AlwaysLoadOnServer = true;
		cabelo->bOwnerNoSee = false;
		cabelo->bAffectDynamicIndirectLighting = true;
		cabelo->PrimaryComponentTick.TickGroup = TG_DuringPhysics;
		cabelo->SetupAttachment(GetSprite());
		static FName CollisionProfileName(TEXT("OverlapAll"));
		cabelo->SetCollisionProfileName(CollisionProfileName);
		cabelo->SetGenerateOverlapEvents(false);
		cabelo->SetCanEverAffectNavigation(false);
		cabelo->SetIsReplicated(false);
	}
	olho = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("olho"));
	if (olho)
	{
		olho->AlwaysLoadOnClient = true;
		olho->AlwaysLoadOnServer = true;
		olho->bOwnerNoSee = false;
		olho->bAffectDynamicIndirectLighting = true;
		olho->PrimaryComponentTick.TickGroup = TG_DuringPhysics;
		olho->SetupAttachment(GetSprite());
		static FName CollisionProfileName(TEXT("OverlapAll"));
		olho->SetCollisionProfileName(CollisionProfileName);
		olho->SetGenerateOverlapEvents(false);
		olho->SetCanEverAffectNavigation(false);
		olho->SetIsReplicated(false);
	}
	luva = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("luva"));
	if (luva)
	{
		luva->AlwaysLoadOnClient = true;
		luva->AlwaysLoadOnServer = true;
		luva->bOwnerNoSee = false;
		luva->bAffectDynamicIndirectLighting = true;
		luva->PrimaryComponentTick.TickGroup = TG_DuringPhysics;
		luva->SetupAttachment(GetSprite());
		static FName CollisionProfileName(TEXT("OverlapAll"));
		luva->SetCollisionProfileName(CollisionProfileName);
		luva->SetGenerateOverlapEvents(false);
		luva->SetCanEverAffectNavigation(false);
		luva->SetIsReplicated(false);
	}
	pele = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("pele"));
	if (pele)
	{
		pele->AlwaysLoadOnClient = true;
		pele->AlwaysLoadOnServer = true;
		pele->bOwnerNoSee = false;
		pele->bAffectDynamicIndirectLighting = true;
		pele->PrimaryComponentTick.TickGroup = TG_DuringPhysics;
		pele->SetupAttachment(GetSprite());
		static FName CollisionProfileName(TEXT("OverlapAll"));
		pele->SetCollisionProfileName(CollisionProfileName);
		pele->SetGenerateOverlapEvents(false);
		pele->SetCanEverAffectNavigation(false);
		pele->SetIsReplicated(false);
	}
	roupa1 = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("roupa1"));
	if (roupa1)
	{
		roupa1->AlwaysLoadOnClient = true;
		roupa1->AlwaysLoadOnServer = true;
		roupa1->bOwnerNoSee = false;
		roupa1->bAffectDynamicIndirectLighting = true;
		roupa1->PrimaryComponentTick.TickGroup = TG_DuringPhysics;
		roupa1->SetupAttachment(GetSprite());
		static FName CollisionProfileName(TEXT("OverlapAll"));
		roupa1->SetCollisionProfileName(CollisionProfileName);
		roupa1->SetGenerateOverlapEvents(false);
		roupa1->SetCanEverAffectNavigation(false);
		roupa1->SetIsReplicated(false);
	}
	roupa2 = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("roupa2"));
	if (roupa2)
	{
		roupa2->AlwaysLoadOnClient = true;
		roupa2->AlwaysLoadOnServer = true;
		roupa2->bOwnerNoSee = false;
		roupa2->bAffectDynamicIndirectLighting = true;
		roupa2->PrimaryComponentTick.TickGroup = TG_DuringPhysics;
		roupa2->SetupAttachment(GetSprite());
		static FName CollisionProfileName(TEXT("OverlapAll"));
		roupa2->SetCollisionProfileName(CollisionProfileName);
		roupa2->SetGenerateOverlapEvents(false);
		roupa2->SetCanEverAffectNavigation(false);
		roupa2->SetIsReplicated(false);
	}
	chuteira = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("chuteira"));
	if (chuteira)
	{
		chuteira->AlwaysLoadOnClient = true;
		chuteira->AlwaysLoadOnServer = true;
		chuteira->bOwnerNoSee = false;
		chuteira->bAffectDynamicIndirectLighting = true;
		chuteira->PrimaryComponentTick.TickGroup = TG_DuringPhysics;
		chuteira->SetupAttachment(GetSprite());
		static FName CollisionProfileName(TEXT("OverlapAll"));
		chuteira->SetCollisionProfileName(CollisionProfileName);
		chuteira->SetGenerateOverlapEvents(false);
		chuteira->SetCanEverAffectNavigation(false);
		chuteira->SetIsReplicated(false);
	}

	GetSprite()->SetRelativeLocation(FVector(3.7, 0.0001, 10.5));
	GetSprite()->SetRelativeScale3D(FVector(0.44));

	// body
	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(82);
	GetCapsuleComponent()->SetCapsuleRadius(1);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("corpo"));
	GetCapsuleComponent()->SetIsReplicated(false);

	cabeca = CreateDefaultSubobject<USphereComponent>(TEXT("cabeca"));
	cabeca->SetRelativeLocation(FVector(0, 0, 59.2));
	cabeca->InitSphereRadius(32.0f);
    cabeca->SetCollisionProfileName(TEXT("corpo"));
	cabeca->SetupAttachment(RootComponent);
	cabeca->SetIsReplicated(false);

	peito = CreateDefaultSubobject<UCapsuleComponent>(TEXT("peito"));
	peito->SetRelativeLocation(FVector(0, 0, 10.4));
	peito->SetCapsuleHalfHeight(56);
	peito->SetCapsuleRadius(27);
	peito->SetCollisionProfileName(TEXT("corpo"));
	peito->SetupAttachment(RootComponent);
	peito->SetIsReplicated(false);

	pernas = CreateDefaultSubobject<USphereComponent>(TEXT("pernas"));
	pernas->SetRelativeLocation(FVector(0, 0, -42.9));
	pernas->InitSphereRadius(40.0f);
	pernas->SetCollisionProfileName(TEXT("corpo"));
	pernas->SetupAttachment(RootComponent);
	pernas->SetIsReplicated(false);

	pes = CreateDefaultSubobject<UBoxComponent>(TEXT("pes"));
	pes->SetRelativeLocation(FVector(0, 0, -75.6));
	pes->SetBoxExtent(FVector(50, 40, 2));
	pes->SetCollisionProfileName(TEXT("corpo"));
	pes->SetupAttachment(RootComponent);
	pes->SetIsReplicated(false);

	chute_angulo = CreateDefaultSubobject<UBoxComponent>(TEXT("chute_angulo"));
	chute_angulo->SetRelativeLocation(FVector(42.0, 0, -74));
	chute_angulo->SetRelativeScale3D(FVector(0.02, 1, 0.6125));
	chute_angulo->SetBoxExtent(FVector(32, 32, 32));
	chute_angulo->SetCollisionProfileName(TEXT("NoCollision"));
	chute_angulo->SetupAttachment(RootComponent);
	chute_angulo->SetIsReplicated(false);

	pode_cabecear = CreateDefaultSubobject<UBoxComponent>(TEXT("pode_cabecear"));
	pode_cabecear->SetRelativeLocation(FVector(32, 0, 86));
	pode_cabecear->SetRelativeScale3D(FVector(1, 1, 1));
	pode_cabecear->SetBoxExtent(FVector(32, 32, 42));
	pode_cabecear->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	pode_cabecear->SetupAttachment(RootComponent);
	pode_cabecear->OnComponentBeginOverlap.AddDynamic(this, &AOJogoCharacter::cabecearBeginOverlap); 
	pode_cabecear->OnComponentEndOverlap.AddDynamic(this, &AOJogoCharacter::cabecearEndOverlap);
	pode_cabecear->SetIsReplicated(false);

	pode_chutar = CreateDefaultSubobject<UBoxComponent>(TEXT("pode_chutar"));
	pode_chutar->SetRelativeLocation(FVector(32, 0, -32));
	pode_chutar->SetRelativeScale3D(FVector(2, 1, 1));
	pode_chutar->SetBoxExtent(FVector(32, 32, 42));
	pode_chutar->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	pode_chutar->SetupAttachment(RootComponent);
	pode_chutar->OnComponentBeginOverlap.AddDynamic(this, &AOJogoCharacter::chutarBeginOverlap); 
	pode_chutar->OnComponentEndOverlap.AddDynamic(this, &AOJogoCharacter::chutarEndOverlap);
	pode_chutar->SetIsReplicated(false);

	staminaRT = 100.0f;
	forcaChuteRT = 0.0f;
	
	index_controller = 0;
    bisMovingRight = true;
    kicking = false;
    sliding = false;
    onAir = false;
    canHeader = false;
    canKick = false;
	inputEnabled = true;
	dashFinished = true;
	slidingActionFinished = true;
	chutaFinished = true;
    auxSpeed = 0.0f;
    auxAcceleration = 0.0f;
    chute_location = FVector(0, 0, 0);
}

void AOJogoCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOJogoCharacter, maxForcaChute);
	DOREPLIFETIME(AOJogoCharacter, maxForcaCabeceio);
	DOREPLIFETIME(AOJogoCharacter, velocidade);
	DOREPLIFETIME(AOJogoCharacter, velocidadeCarrinho);
	DOREPLIFETIME(AOJogoCharacter, aceleracaoCarrinho);
	DOREPLIFETIME(AOJogoCharacter, staminaRegen);
	DOREPLIFETIME(AOJogoCharacter, jumpStaminaCost);
	DOREPLIFETIME(AOJogoCharacter, slidingStaminaCost);

	DOREPLIFETIME(AOJogoCharacter, inputEnabled);
	DOREPLIFETIME(AOJogoCharacter, kicking);
	DOREPLIFETIME(AOJogoCharacter, sliding);
}

void AOJogoCharacter::BeginPlay()
{
    Super::BeginPlay();

	chute_location = chute_angulo->GetRelativeLocation();
}

//////////////////////////////////////////////////////////////////////////
// Animation
void AOJogoCharacter::UpdateAnimation()
{
	/*if (HasAuthority())
		GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, FString::Printf(TEXT("Server: update %.2f"), maxForcaChute));
	else
		GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Emerald, FString::Printf(TEXT("Client: update %.2f"), maxForcaChute));*/

	const FVector PlayerVelocity = GetVelocity();

	// usar caso tenha sprite de falling
	// bool falling = PlayerVelocity.Z < 0; 
	if (kicking)
		setMovimentacao(2);
	else if (sliding)
		setMovimentacao(4);
	else if (onAir)
		setMovimentacao(1);
	else
	{
		if (UKismetMathLibrary::VSize(PlayerVelocity) > 0)
			setMovimentacao(3);
		else
			setMovimentacao(0);
	}
}

void AOJogoCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	onAir = GetCharacterMovement()->IsFalling();

	if (sliding && dashFinished)
	{
		dashFinished = false;
		setDashFinished(false);
		dashFunction();
	}
	else
		UpdateAnimation();
}

void AOJogoCharacter::MoveRight(float Value)
{
	if (Value > 0)
		bisMovingRight = true;
	else if (Value < 0)
		bisMovingRight = false;
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AOJogoCharacter::Pula()
{
	if (inputEnabled)
	{
		if (GetStaminaRT() > jumpStaminaCost)
		{
			if (!onAir && !sliding)
			{
				// stamina = stamina - jumpStaminaCost;
				float s = GetStaminaRT() - jumpStaminaCost;
				// GetPlayerState<APlayerCharacterState>()->SV_SetStamina(s);
				// staminaRT = s;
				SetStaminaRT(s);
				ACharacter::Jump();
			}
		}
	}
}

void AOJogoCharacter::Chuta()
{
	if (!sliding)
	{
		if (chutaFinished)
		{
			chutaFinished = false;
			kicking = true;
			setKicking(true);

			float forca = setForcaChute();
			SV_Chuta(chute_angulo->GetComponentRotation(), forca);

			/*TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABola::StaticClass(), FoundActors);
			if (FoundActors.Num() == 1)
				ball = Cast<ABola>(FoundActors[0]);

			if (IsValid(ball) && (canKick || canHeader))
				ball->Predict_chuta(chute_angulo->GetComponentRotation(), forca);*/
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoCharacter::chutaTimeout, 0.2f, false);
		}
	}
}

void AOJogoCharacter::SV_Chuta_Implementation(FRotator anguloChute, float forca)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABola::StaticClass(), FoundActors);
	if (FoundActors.Num() == 1)
		ball = Cast<ABola>(FoundActors[0]);

	if (IsValid(ball) && (canKick || canHeader))
		ball->SV_chuta(anguloChute, forca);
}

void AOJogoCharacter::chutaTimeout()
{
	kicking = false;
	setKicking(false);
	chutaFinished = true;
}

void AOJogoCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void AOJogoCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void AOJogoCharacter::setHabilidades_Implementation(const FHabilidadesData h)
{
	MC_setHabilidades(h);
}

void AOJogoCharacter::MC_setHabilidades_Implementation(const FHabilidadesData h)
{
	maxForcaChute = h.converteMaxForcaChute();
	maxForcaCabeceio = h.converteMaxForcaCabeceio();

	velocidade = h.converteVelocidade();
	GetCharacterMovement()->MaxWalkSpeed = h.converteVelocidade();

	velocidadeCarrinho = h.converteVelocidadeCarrinho();
	aceleracaoCarrinho = h.converteAceleracaoCarrinho();
	staminaRegen = h.converteStaminaRegen();
	jumpStaminaCost = h.converteJumpStaminaCost();
	slidingStaminaCost = h.converteSlidingStaminaCost();
}

void AOJogoCharacter::setJogador_Implementation(FJogadorData f)
{
	MC_setJogador(f);
}

void AOJogoCharacter::MC_setJogador_Implementation(FJogadorData f)
{
	jogador = f;
	cabelo->SetSpriteColor(jogador.cabelo);
	olho->SetSpriteColor(jogador.olho);
	luva->SetSpriteColor(jogador.luva);
	pele->SetSpriteColor(jogador.pele);
	roupa1->SetSpriteColor(jogador.roupa1);
	roupa2->SetSpriteColor(jogador.roupa2);
	chuteira->SetSpriteColor(jogador.chuteira);
}

void AOJogoCharacter::pawnConfig_Implementation(FJogadorData f, FHabilidadesData h)
{
	// GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Emerald, FString::Printf(TEXT("PawnConfig")));
	UE_LOG(LogTemp, Warning, TEXT("PawnConfig"));
	setHabilidades(h);
	setJogador(f);

	//if (IsLocallyControlled())
	//{
	//	FTimerHandle UnusedHandle;
	//	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoCharacter::staminaRegenLoop, 0.01f, true); // 0.05
	//}
}

void AOJogoCharacter::setMovimentacao(int mIndex)
{
	if (jogador.cabeloArray.Num() > 0)
	{
		cabelo->SetFlipbook(jogador.cabeloArray[mIndex]);
		olho->SetFlipbook(jogador.olhoArray[mIndex]);
		luva->SetFlipbook(jogador.luvaArray[mIndex]);
		pele->SetFlipbook(jogador.peleArray[mIndex]);
		roupa1->SetFlipbook(jogador.roupa1Array[mIndex]);
		roupa2->SetFlipbook(jogador.roupa2Array[mIndex]);
		chuteira->SetFlipbook(jogador.chuteiraArray[mIndex]);
	}
}

void AOJogoCharacter::dashFunction()
{
	// APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), index_controller);
	// DisableInput(PC);
	inputEnabled = false;

	auxAcceleration = GetCharacterMovement()->BrakingFrictionFactor;
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	LaunchCharacter(3 * velocidadeCarrinho * FVector(GetActorForwardVector().X, 0, 0).GetSafeNormal(), true, true);
	SV_LaunchCharacter();

	// GetWorldTimerManager().SetTimer(dashHandle, this, &AOJogoCharacter::dashing, 0.005f, false);

	// se mudar o tempo de dashing mudar o tempo de congelamento de cerebro no bot tambem
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AOJogoCharacter::terminaDashing, 0.8f, false);
}

void AOJogoCharacter::SV_LaunchCharacter_Implementation()
{
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	LaunchCharacter(3 * velocidadeCarrinho * FVector(GetActorForwardVector().X, 0, 0).GetSafeNormal(), true, true);
}

void AOJogoCharacter::dashing() // deprecated
{
	AddMovementInput(GetCapsuleComponent()->GetForwardVector(), 100.0f);
}

void AOJogoCharacter::terminaDashing()
{
	GetCharacterMovement()->BrakingFrictionFactor = auxAcceleration;

	// APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), index_controller);
	// EnableInput(PC);
	stopSliding();
	inputEnabled = true;
	dashFinished = true;
	setDashFinished(true);
}

void AOJogoCharacter::slideAction()
{
	if (!kicking)
	{
		if (slidingActionFinished)
		{
			slidingActionFinished = false;
			setSlidingActionFinished(false);
			if (GetStaminaRT() > slidingStaminaCost)
			{
				if (onAir)
					stopSliding();
				else
				{
					// stamina = stamina - slidingStaminaCost;
					float s = GetStaminaRT() - slidingStaminaCost;
					// GetPlayerState<APlayerCharacterState>()->SV_SetStamina(s);
					// staminaRT = s;
					SetStaminaRT(s);
					sliding = true;
					setSliding(true);
				}
			}
			else
				stopSliding();
		}
	}
}

void AOJogoCharacter::stopSliding()
{
    slidingActionFinished = true;
	setSlidingActionFinished(true);
	sliding = false;
	setSliding(false);
}

void AOJogoCharacter::setSliding_Implementation(bool b)
{
	sliding = b;
}

void AOJogoCharacter::setDashFinished_Implementation(bool b)
{
	dashFinished = b;
}

void AOJogoCharacter::setKicking_Implementation(bool b)
{
	kicking = b;
}

void AOJogoCharacter::setSlidingActionFinished_Implementation(bool b)
{
	slidingActionFinished = b;
}

float AOJogoCharacter::setForcaChute()
{
	if (canKick)
	{
		chute_angulo->SetRelativeLocation(chute_location);
		return GetForcaChuteRT() * maxForcaChute;
	}
	else
	{
		chute_angulo->SetRelativeLocation(cabeca->GetRelativeLocation());
		// float theta = 0;
		// if (ball)
		// 	theta = (ball->GetActorLocation() - cabeca->GetComponentLocation()).X;
		// theta = 90 - ((90 * (UKismetMathLibrary::Abs(theta)))/theta);
		// chute_angulo->SetWorldRotation(FRotator(theta, 0, 0));
		return GetForcaChuteRT() * maxForcaCabeceio;
	}
}

void AOJogoCharacter::cabecearBeginOverlap(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, 
										   UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, 
										   bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		auto bola_temp = Cast<ABola>(OtherActor);
		if (IsValid(bola_temp))
			if (bola_temp->GetRootComponent() == OtherComp)
				canHeader = true;
	}
}

void AOJogoCharacter::cabecearEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		auto bola_temp = Cast<ABola>(OtherActor);
		if (IsValid(bola_temp))
			if (bola_temp->GetRootComponent() == OtherComp)
				canHeader = false;
	}
}

void AOJogoCharacter::chutarBeginOverlap(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, 
										 UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, 
										 bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		auto bola_temp = Cast<ABola>(OtherActor);
		if (IsValid(bola_temp))
			if (bola_temp->GetRootComponent() == OtherComp)
				canKick = true;
	}
}

void AOJogoCharacter::chutarEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
									   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		auto bola_temp = Cast<ABola>(OtherActor);
		if (IsValid(bola_temp))
			if (bola_temp->GetRootComponent() == OtherComp)
				canKick = false;
	}
}