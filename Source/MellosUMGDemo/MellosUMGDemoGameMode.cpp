// Copyright Epic Games, Inc. All Rights Reserved.

#include "MellosUMGDemoGameMode.h"
#include "MellosUMGDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMellosUMGDemoGameMode::AMellosUMGDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
