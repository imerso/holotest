// Fill out your copyright notice in the Description page of Project Settings.


#include "HolotestPlayerState.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <Holotest/PlayerChar.h>


// Constructor
AHolotestPlayerState::AHolotestPlayerState()
{
	// Clear hits count
	Energy = 10;
}

// Damage and decrease player energy
uint16 AHolotestPlayerState::PlayerDamage()
{
	if (HasAuthority())
	{
		// Decreases until no energy is available
		if (Energy)
		{
			Energy--;
			UE_LOG(LogTemp, Warning, TEXT("REMAINING ENERGY: %u"), Energy);
		}

		// Test again because it
		// could be a different shot while
		// the player is still showing on scene
		if (!Energy)
		{
			// Player is dead...
			UE_LOG(LogTemp, Warning, TEXT("DEAD PLAYER..."));
			GetPawn()->Destroy();
		}
	}

	return Energy;
}

// Client Energy replication update
void AHolotestPlayerState::OnRep_Energy()
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT ENERGY: %u"), Energy);
}

// New Client actual player state variables replication
void AHolotestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHolotestPlayerState, Energy);
	UE_LOG(LogTemp, Warning, TEXT("NEW CLIENT REPLICATE ENERGY: %u"), Energy);
}
