// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "JogadorData.h"
#include "Bola.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "HabilidadesData.h"
#include "OJogoCharacter.generated.h"

class UTextRenderComponent;

/**
 * This class is the default character for OJogo, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class AOJogoCharacter : public APaperCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// UTextRenderComponent* TextComponent;

	UPROPERTY()
	bool dashFinished;

	UPROPERTY()
	bool slidingActionFinished;

	UPROPERTY()
	bool chutaFinished;

	UPROPERTY()
	FTimerHandle dashHandle;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ABola* ball;
	
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbookComponent* cabelo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbookComponent* olho;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbookComponent* luva;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbookComponent* pele;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbookComponent* roupa1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbookComponent* roupa2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbookComponent* chuteira;

	UPROPERTY(BlueprintReadWrite, Category = "Habilidades")
	float maxForcaChute;

	UPROPERTY(BlueprintReadWrite, Category = "Habilidades")
	float maxForcaCabeceio;

	UPROPERTY(BlueprintReadWrite, Category = "Habilidades")
	float velocidade;

	UPROPERTY(BlueprintReadWrite, Category = "Habilidades")
	float velocidadeCarrinho;

	UPROPERTY(BlueprintReadWrite, Category = "Habilidades")
	float aceleracaoCarrinho;

	UPROPERTY(BlueprintReadWrite, Category = "Habilidades")
	float staminaRegen;

	UPROPERTY(BlueprintReadWrite, Category = "Habilidades")
	float jumpStaminaCost;

	UPROPERTY(BlueprintReadWrite, Category = "Habilidades")
	float slidingStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	FJogadorData jogador;

	UPROPERTY(BlueprintReadWrite)
	float stamina;

	UPROPERTY(BlueprintReadWrite)
	bool bisMovingRight;

	UPROPERTY(BlueprintReadWrite)
	bool kicking;

	UPROPERTY(BlueprintReadWrite)
	bool sliding;

	UPROPERTY(BlueprintReadWrite)
	bool onAir;

	UPROPERTY(BlueprintReadWrite)
	float auxSpeed;

	UPROPERTY(BlueprintReadWrite)
	float auxAcceleration;

	UPROPERTY(BlueprintReadWrite)
	float forca_chute;

	UPROPERTY(BlueprintReadWrite)
	bool canHeader;

	UPROPERTY(BlueprintReadWrite)
	bool canKick;

	UPROPERTY(BlueprintReadWrite)
	bool inputEnabled;

	UPROPERTY(BlueprintReadWrite)
	FVector chute_location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* cabeca;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* peito;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* pernas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* pes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* chute_angulo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* pode_chutar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* pode_cabecear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 index_controller;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	void chutaTimeout();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

public:
	AOJogoCharacter();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable)
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
	void Pula();

	UFUNCTION(BlueprintCallable)
	void Chuta();

	FORCEINLINE void SetIndexController(int32 index) { index_controller = index; }

	FORCEINLINE int32 GetIndexController() const { return index_controller; }

	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UPaperFlipbookComponent* Getcabelo() const { return cabelo; }

	FORCEINLINE class UPaperFlipbookComponent* Getolho() const { return olho; }

	FORCEINLINE class UPaperFlipbookComponent* Getluva() const { return luva; }

	FORCEINLINE class UPaperFlipbookComponent* Getpele() const { return pele; }

	FORCEINLINE class UPaperFlipbookComponent* Getroupa1() const { return roupa1; }

	FORCEINLINE class UPaperFlipbookComponent* Getroupa2() const { return roupa2; }

	FORCEINLINE class UPaperFlipbookComponent* Getchuteira() const { return chuteira; }

	FORCEINLINE class UBoxComponent* Getchute_angulo() const { return chute_angulo; }

	UFUNCTION(BlueprintCallable, Category = "Habilidades")
	void setVelocidade(float v);

	UFUNCTION(BlueprintCallable, Category = "Habilidades")
	void setHabilidades(const FHabilidadesData h);

	UFUNCTION(BlueprintCallable, Category = "Habilidades")
	void setJogador(FJogadorData f);

	UFUNCTION(BlueprintCallable, Category = "Habilidades")
	void setMovimentacao(int mIndex);

	UFUNCTION(BlueprintCallable, Category = "Habilidades")
	void setCor();

	UFUNCTION(BlueprintCallable)
	void staminaRegenLoop();

	UFUNCTION(BlueprintCallable)
	void dashFunction();

	UFUNCTION(BlueprintCallable)
	void dashing();

	UFUNCTION(BlueprintCallable)
	void terminaDashing();

	UFUNCTION(BlueprintCallable)
	void slideAction();

	UFUNCTION(BlueprintCallable)
	void stopSliding();

	UFUNCTION(BlueprintCallable)
	float setForcaChute();

	UFUNCTION(BlueprintCallable)
	void cabecearBeginOverlap(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, 
							  UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, 
							  bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
	void cabecearEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void chutarBeginOverlap(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, 
							UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, 
							bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
	void chutarEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
