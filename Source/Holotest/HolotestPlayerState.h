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
	uint16 Energy;

	// Client energy replication update
	UFUNCTION()
	void OnRep_Energy();

	// Player score
	UPROPERTY(ReplicatedUsing = OnRep_PScore)
	uint32 PScore;

	// Client score replication update
	UFUNCTION()
	void OnRep_PScore();

public:

	// Constructor
	AHolotestPlayerState();

	// Damage and decrease player energy
	void PlayerDamage(uint16 Amount);

	// Add score
	void PlayerAddScore(uint32 Amount);

	uint16 GetEnergy() { return Energy; }
	uint32 GetScore() { return PScore; }
	void Reset() { Energy = 100; }
};
