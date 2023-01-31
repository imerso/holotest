// Fill out your copyright notice in the Description page of Project Settings.


#include "HolotestGameStateBase.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>


// Constructs
AHolotestGameStateBase::AHolotestGameStateBase()
{
	HitsCount = 0;
}

// Increases total hits count
void AHolotestGameStateBase::PlayerHit()
{
	if (HasAuthority())
	{
		HitsCount++;
		UE_LOG(LogTemp, Warning, TEXT("SERVER HITS COUNT: %u"), HitsCount);
	}
}

// Client HitsCount replication update
void AHolotestGameStateBase::OnRep_HitsCount()
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT HITS COUNT: %u"), HitsCount);
}

// New Client actual game state variables replication
void AHolotestGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHolotestGameStateBase, HitsCount);
	UE_LOG(LogTemp, Warning, TEXT("NEW CLIENT REPLICATE HITS COUNT: %u"), HitsCount);
}
