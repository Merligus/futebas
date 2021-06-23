// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "JogadorData.h"
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

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;
protected:
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
	float MaxForcaChute;

	UPROPERTY(BlueprintReadWrite, Category = "Habilidades")
	float MaxForcaCabeceio;

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

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	AOJogoCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UPaperFlipbookComponent* Getcabelo() const { return cabelo; }

	FORCEINLINE class UPaperFlipbookComponent* Getolho() const { return olho; }

	FORCEINLINE class UPaperFlipbookComponent* Getluva() const { return luva; }

	FORCEINLINE class UPaperFlipbookComponent* Getpele() const { return pele; }

	FORCEINLINE class UPaperFlipbookComponent* Getroupa1() const { return roupa1; }

	FORCEINLINE class UPaperFlipbookComponent* Getroupa2() const { return roupa2; }

	FORCEINLINE class UPaperFlipbookComponent* Getchuteira() const { return chuteira; }

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
};
