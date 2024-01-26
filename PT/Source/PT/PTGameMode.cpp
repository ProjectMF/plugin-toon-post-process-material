// Copyright Epic Games, Inc. All Rights Reserved.

#include "PTGameMode.h"
#include "PTCharacter.h"
#include "UObject/ConstructorHelpers.h"

APTGameMode::APTGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
