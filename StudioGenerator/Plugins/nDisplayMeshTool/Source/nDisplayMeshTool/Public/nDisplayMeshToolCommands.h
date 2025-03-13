// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "nDisplayMeshToolStyle.h"

class FnDisplayMeshToolCommands : public TCommands<FnDisplayMeshToolCommands>
{
public:

	FnDisplayMeshToolCommands()
		: TCommands<FnDisplayMeshToolCommands>(TEXT("nDisplayMeshTool"), NSLOCTEXT("Contexts", "nDisplayMeshTool", "nDisplayMeshTool Plugin"), NAME_None, FnDisplayMeshToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};