// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GKLabProject.h"
#include "GKLabProjectGameMode.h"
#include "GKLabProjectHUD.h"
#include "GKLabProjectCharacter.h"

AGKLabProjectGameMode::AGKLabProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGKLabProjectHUD::StaticClass();
}
