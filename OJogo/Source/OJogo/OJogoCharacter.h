// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "OJogoCharacter.generated.h"

class UTextRenderComponent;


USTRUCT(BlueprintType)
struct FHabilidades : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
		float MaxForcaChute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
		float MaxForcaCabeceio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
		float velocidade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
		float velocidadeCarrinho;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
		float aceleracaoCarrinho;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
		float staminaRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
		float jumpStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habilidades")
		float slidingStaminaCost;

	FHabilidades()
	{
		MaxForcaChute = 5000.0f;
		MaxForcaCabeceio = 5000.0f;
		velocidade = 600.0f;
		velocidadeCarrinho = 1200.0f;
		aceleracaoCarrinho = 10000.0f;
		staminaRegen = 0.05f;
		jumpStaminaCost = 5.0f;
		slidingStaminaCost = 20.0f;
	}

	~FHabilidades()
	{

	}
};

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

	UFUNCTION(BlueprintCallable, Category = "Habilidades")
	void setVelocidade(float v);

	UFUNCTION(BlueprintCallable, Category = "Habilidades")
	void setHabilidades(const FHabilidades h);
};
