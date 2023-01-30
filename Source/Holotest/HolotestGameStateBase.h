// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HolotestGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class HOLOTEST_API AHolotestGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	// Replicated HitsCount
	UPROPERTY(ReplicatedUsing = OnRep_HitsCount)
	uint16 HitsCount;

	// Client HitsCount replication update
	UFUNCTION()
	void OnRep_HitsCount();

public:

	// Constructs
	AHolotestGameStateBase();

	// Inflict player damage
	void PlayerHit();

};
