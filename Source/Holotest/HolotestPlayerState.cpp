// Fill out your copyright notice in the Description page of Project Settings.


#include "HolotestPlayerState.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>


// Constructor
AHolotestPlayerState::AHolotestPlayerState()
{
	// Clear hits count
	Energy = 10;
}

// Damage and decrease player energy
void AHolotestPlayerState::PlayerDamage()
{
	if (HasAuthority())
	{
		Energy--;
		UE_LOG(LogTemp, Warning, TEXT("REMAINING ENERGY: %u"), Energy);

		if (Energy <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("DEAD!"));
		}
	}
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
