// Copyright Epic Games, Inc. All Rights Reserved.

#include "automationplayg5_5GameMode.h"
#include "automationplayg5_5Character.h"
#include "UObject/ConstructorHelpers.h"

Aautomationplayg5_5GameMode::Aautomationplayg5_5GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
