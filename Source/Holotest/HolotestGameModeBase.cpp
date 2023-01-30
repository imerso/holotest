// Copyright Epic Games, Inc. All Rights Reserved.


#include "HolotestGameModeBase.h"
#include "HolotestGameStateBase.h"


AHolotestGameModeBase::AHolotestGameModeBase()
{

}

// Inflict player damage
void AHolotestGameModeBase::PlayerHit()
{
	AHolotestGameStateBase* GState = GetGameState<AHolotestGameStateBase>();
	if (GState)
	{
		GState->PlayerHit();
	}
}
