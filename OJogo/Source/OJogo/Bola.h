// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundWave.h"
#include "Net/UnrealNetwork.h"
#include "Bola.generated.h"

UCLASS()
class OJOGO_API ABola : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	FVector current_pos;

	UPROPERTY()
	FVector current_vel;

	UPROPERTY()
	float t_no_updates;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Bola")
	USphereComponent* esfera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Bola")
	UStaticMeshComponent* aparencia;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbookComponent* explosao;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sons)
	USoundWave* som_chute;

	// Sets default values for this actor's properties
	ABola();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void UpdateBall(FVector posicao, FVector velocidade);
	void UpdateBall_Implementation(FVector posicao, FVector velocidade);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void explode(FVector posicao);
	void explode_Implementation(FVector posicao);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void fazSomChute(FVector posicao);
	void fazSomChute_Implementation(FVector posicao);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SV_chuta(FRotator anguloChute, float forca);
	void SV_chuta_Implementation(FRotator anguloChute, float forca);
};
