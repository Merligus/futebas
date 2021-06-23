// Copyright Epic Games, Inc. All Rights Reserved.

#include "OJogoCharacter.h"
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

AOJogoCharacter::AOJogoCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
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
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;


	MaxForcaChute = 5000.0f;
	MaxForcaCabeceio = 5000.0f;
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
		cabelo->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		cabelo->SetupAttachment(Sprite);
		static FName CollisionProfileName(TEXT("OverlapAll"));
		cabelo->SetCollisionProfileName(CollisionProfileName);
		cabelo->SetGenerateOverlapEvents(false);
	}
	olho = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("olho"));
	if (olho)
	{
		olho->AlwaysLoadOnClient = true;
		olho->AlwaysLoadOnServer = true;
		olho->bOwnerNoSee = false;
		olho->bAffectDynamicIndirectLighting = true;
		olho->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		olho->SetupAttachment(Sprite);
		static FName CollisionProfileName(TEXT("OverlapAll"));
		olho->SetCollisionProfileName(CollisionProfileName);
		olho->SetGenerateOverlapEvents(false);
	}
	luva = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("luva"));
	if (luva)
	{
		luva->AlwaysLoadOnClient = true;
		luva->AlwaysLoadOnServer = true;
		luva->bOwnerNoSee = false;
		luva->bAffectDynamicIndirectLighting = true;
		luva->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		luva->SetupAttachment(Sprite);
		static FName CollisionProfileName(TEXT("OverlapAll"));
		luva->SetCollisionProfileName(CollisionProfileName);
		luva->SetGenerateOverlapEvents(false);
	}
	pele = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("pele"));
	if (pele)
	{
		pele->AlwaysLoadOnClient = true;
		pele->AlwaysLoadOnServer = true;
		pele->bOwnerNoSee = false;
		pele->bAffectDynamicIndirectLighting = true;
		pele->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		pele->SetupAttachment(Sprite);
		static FName CollisionProfileName(TEXT("OverlapAll"));
		pele->SetCollisionProfileName(CollisionProfileName);
		pele->SetGenerateOverlapEvents(false);
	}
	roupa1 = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("roupa1"));
	if (roupa1)
	{
		roupa1->AlwaysLoadOnClient = true;
		roupa1->AlwaysLoadOnServer = true;
		roupa1->bOwnerNoSee = false;
		roupa1->bAffectDynamicIndirectLighting = true;
		roupa1->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		roupa1->SetupAttachment(Sprite);
		static FName CollisionProfileName(TEXT("OverlapAll"));
		roupa1->SetCollisionProfileName(CollisionProfileName);
		roupa1->SetGenerateOverlapEvents(false);
	}
	roupa2 = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("roupa2"));
	if (roupa2)
	{
		roupa2->AlwaysLoadOnClient = true;
		roupa2->AlwaysLoadOnServer = true;
		roupa2->bOwnerNoSee = false;
		roupa2->bAffectDynamicIndirectLighting = true;
		roupa2->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		roupa2->SetupAttachment(Sprite);
		static FName CollisionProfileName(TEXT("OverlapAll"));
		roupa2->SetCollisionProfileName(CollisionProfileName);
		roupa2->SetGenerateOverlapEvents(false);
	}
	chuteira = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("chuteira"));
	if (chuteira)
	{
		chuteira->AlwaysLoadOnClient = true;
		chuteira->AlwaysLoadOnServer = true;
		chuteira->bOwnerNoSee = false;
		chuteira->bAffectDynamicIndirectLighting = true;
		chuteira->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		chuteira->SetupAttachment(Sprite);
		static FName CollisionProfileName(TEXT("OverlapAll"));
		chuteira->SetCollisionProfileName(CollisionProfileName);
		chuteira->SetGenerateOverlapEvents(false);
	}
}

//////////////////////////////////////////////////////////////////////////
// Animation

void AOJogoCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AOJogoCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	/*
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("%f %f %f %f %f %f"), MaxForcaChute, MaxForcaCabeceio, velocidade, velocidadeCarrinho, staminaRegen, jumpStaminaCost, slidingStaminaCost));
	*/

	UpdateCharacter();	
}


//////////////////////////////////////////////////////////////////////////
// Input

void AOJogoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOJogoCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AOJogoCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AOJogoCharacter::TouchStopped);
}

void AOJogoCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
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

void AOJogoCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

void AOJogoCharacter::setHabilidades(const FHabilidades h)
{
	this->MaxForcaChute = h.MaxForcaChute;
	this->MaxForcaCabeceio = h.MaxForcaCabeceio;
	this->setVelocidade(h.velocidade);
	this->velocidadeCarrinho = h.velocidadeCarrinho;
	this->aceleracaoCarrinho = h.aceleracaoCarrinho;
	this->staminaRegen = h.staminaRegen;
	this->jumpStaminaCost = h.jumpStaminaCost;
	this->slidingStaminaCost = h.slidingStaminaCost;
}

void AOJogoCharacter::setVelocidade(float v)
{
	this->velocidade = v;
	GetCharacterMovement()->MaxWalkSpeed = v;
}

void AOJogoCharacter::setJogador(FJogadorData f)
{
	jogador = f;
	this->setColor();
}

void AOJogoCharacter::setColor()
{
	cabelo->SetSpriteColor(jogador.cabelo);
	olho->SetSpriteColor(jogador.olho);
	luva->SetSpriteColor(jogador.luva);
	pele->SetSpriteColor(jogador.pele);
	roupa1->SetSpriteColor(jogador.roupa1);
	roupa2->SetSpriteColor(jogador.roupa2);
	chuteira->SetSpriteColor(jogador.chuteira);
}

void AOJogoCharacter::setMove(int mIndex)
{
	cabelo->SetFlipbook(jogador.cabeloArray[mIndex]);
	olho->SetFlipbook(jogador.olhoArray[mIndex]);
	luva->SetFlipbook(jogador.luvaArray[mIndex]);
	pele->SetFlipbook(jogador.peleArray[mIndex]);
	roupa1->SetFlipbook(jogador.roupa1Array[mIndex]);
	roupa2->SetFlipbook(jogador.roupa2Array[mIndex]);
	chuteira->SetFlipbook(jogador.chuteiraArray[mIndex]);
}