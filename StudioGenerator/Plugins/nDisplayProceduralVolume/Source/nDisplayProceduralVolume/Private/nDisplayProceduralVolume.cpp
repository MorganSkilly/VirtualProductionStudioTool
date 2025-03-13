// Copyright Epic Games, Inc. All Rights Reserved.

#include "nDisplayProceduralVolume.h"
#include "nDisplayProceduralVolumeStyle.h"
#include "nDisplayProceduralVolumeCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName nDisplayProceduralVolumeTabName("nDisplayProceduralVolume");

#define LOCTEXT_NAMESPACE "FnDisplayProceduralVolumeModule"

void FnDisplayProceduralVolumeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FnDisplayProceduralVolumeStyle::Initialize();
	FnDisplayProceduralVolumeStyle::ReloadTextures();

	FnDisplayProceduralVolumeCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FnDisplayProceduralVolumeCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FnDisplayProceduralVolumeModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FnDisplayProceduralVolumeModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(nDisplayProceduralVolumeTabName, FOnSpawnTab::CreateRaw(this, &FnDisplayProceduralVolumeModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FnDisplayProceduralVolumeTabTitle", "nDisplayProceduralVolume"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FnDisplayProceduralVolumeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FnDisplayProceduralVolumeStyle::Shutdown();

	FnDisplayProceduralVolumeCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(nDisplayProceduralVolumeTabName);
}

TSharedRef<SDockTab> FnDisplayProceduralVolumeModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FnDisplayProceduralVolumeModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("nDisplayProceduralVolume.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FnDisplayProceduralVolumeModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(nDisplayProceduralVolumeTabName);
}

void FnDisplayProceduralVolumeModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FnDisplayProceduralVolumeCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FnDisplayProceduralVolumeCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FnDisplayProceduralVolumeModule, nDisplayProceduralVolume)