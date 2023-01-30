// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HolotestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HOLOTEST_API AHolotestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AHolotestGameModeBase();

	// Updates hits count
	void PlayerHit();
};
