// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HolotestPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HOLOTEST_API AHolotestPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:

	// Player energy
	UPROPERTY(ReplicatedUsing = OnRep_Energy)
	int16 Energy;

	// Client Energy replication update
	UFUNCTION()
	void OnRep_Energy();

public:

	// Constructor
	AHolotestPlayerState();

	// Damage and decrease player energy
	void PlayerDamage();
};
