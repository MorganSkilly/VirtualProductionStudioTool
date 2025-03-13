// Copyright Epic Games, Inc. All Rights Reserved.

#include "nDisplayProceduralVolumeCommands.h"

#define LOCTEXT_NAMESPACE "FnDisplayProceduralVolumeModule"

void FnDisplayProceduralVolumeCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "nDisplayProceduralVolume", "Bring up nDisplayProceduralVolume window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
