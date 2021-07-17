// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OJogoCharacter.h"
#include "OJogoGameState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BotCharacter.generated.h"

/**
 * 
 */
UCLASS()
class OJOGO_API ABotCharacter : public AOJogoCharacter
{
	GENERATED_BODY()

	UPROPERTY()
	AOJogoGameState* JogosGameState;

protected:

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> posicoes_gols;

	UPROPERTY(BlueprintReadWrite)
	int32 meus_gols;

	UPROPERTY(BlueprintReadWrite)
	int32 gols_adversario;

	UPROPERTY(BlueprintReadWrite)
	int32 resultado_que_precisa;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;

public:

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void clearMoveFinish();

	UFUNCTION(BlueprintCallable)
	void setOwnGoal(int32 pos_index);

	UFUNCTION(BlueprintCallable, Category = "BotIA")
	void setBotGols(int32 meus, int32 adversario);

	UFUNCTION(BlueprintCallable)
	void BotChuta(float forca, float angulo);

	UFUNCTION(BlueprintCallable)
	void cabecearBeginOverlapBot(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, 
							     UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, 
							     bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
	void cabecearEndOverlapBot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void chutarBeginOverlapBot(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, 
							   UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, 
							   bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
	void chutarEndOverlapBot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void horizontalDistance(FVector v1, FVector v2, float& abs, float& d);

	void verticalDistance(FVector v1, FVector v2, float& abs, float& d);

	FVector futurePosBall(FVector location, FVector velocity, float time);

	bool areComponentsNear(FVector A_Location, FVector B_Location, float radius);

	UFUNCTION(BlueprintCallable)
	void sense();
};
