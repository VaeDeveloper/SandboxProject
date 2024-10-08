// Copyright Epic Games, Inc. All Rights Reserved.

#include "SandboxProjectGameMode.h"
#include "SandboxProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASandboxProjectGameMode::ASandboxProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
