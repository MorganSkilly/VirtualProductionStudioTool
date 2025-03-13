// Copyright Epic Games, Inc. All Rights Reserved.

#include "nDisplayMeshToolCommands.h"

#define LOCTEXT_NAMESPACE "FnDisplayMeshToolModule"

void FnDisplayMeshToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "nDisplayMeshTool", "Bring up nDisplayMeshTool window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
