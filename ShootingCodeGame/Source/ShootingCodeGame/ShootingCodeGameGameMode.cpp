// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingCodeGameGameMode.h"
#include "ShootingCodeGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShootingCodeGameGameMode::AShootingCodeGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
