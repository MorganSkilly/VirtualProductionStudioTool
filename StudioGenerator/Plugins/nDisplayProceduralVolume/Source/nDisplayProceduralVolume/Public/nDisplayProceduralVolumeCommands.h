// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "nDisplayProceduralVolumeStyle.h"

class FnDisplayProceduralVolumeCommands : public TCommands<FnDisplayProceduralVolumeCommands>
{
public:

	FnDisplayProceduralVolumeCommands()
		: TCommands<FnDisplayProceduralVolumeCommands>(TEXT("nDisplayProceduralVolume"), NSLOCTEXT("Contexts", "nDisplayProceduralVolume", "nDisplayProceduralVolume Plugin"), NAME_None, FnDisplayProceduralVolumeStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};