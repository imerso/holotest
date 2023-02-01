// Fill out your copyright notice in the Description page of Project Settings.


#include "HolotestPlayerState.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <Holotest/PlayerChar.h>


// Constructor
AHolotestPlayerState::AHolotestPlayerState()
{
	// Clear stats
	Energy = 100;
	PScore = 0;
}

// Damage and decrease player energy
void AHolotestPlayerState::PlayerDamage(int16 Amount)
{
	if (HasAuthority())
	{
		// Decreases until no energy is available
		if (Energy)
		{
			Energy = FMath::Max<int16>(0, Energy - Amount);
		}
	}
}

// Add score - can be negative
void AHolotestPlayerState::PlayerAddScore(int32 Amount)
{
	if (HasAuthority())
	{
		PScore = FMath::Max<int32>(0, PScore + Amount);
	}
}

// Client Energy replication update
void AHolotestPlayerState::OnRep_Energy()
{
}

// Cliente Score replication update
void AHolotestPlayerState::OnRep_PScore()
{
}

// New Client actual player state variables replication
void AHolotestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHolotestPlayerState, Energy);
	DOREPLIFETIME(AHolotestPlayerState, PScore);
}
