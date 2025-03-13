// Copyright Epic Games, Inc. All Rights Reserved.

#include "nDisplayMeshTool.h"
#include "nDisplayMeshToolStyle.h"
#include "nDisplayMeshToolCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName nDisplayMeshToolTabName("nDisplayMeshTool");

#define LOCTEXT_NAMESPACE "FnDisplayMeshToolModule"

void FnDisplayMeshToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FnDisplayMeshToolStyle::Initialize();
	FnDisplayMeshToolStyle::ReloadTextures();

	FnDisplayMeshToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FnDisplayMeshToolCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FnDisplayMeshToolModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FnDisplayMeshToolModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(nDisplayMeshToolTabName, FOnSpawnTab::CreateRaw(this, &FnDisplayMeshToolModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FnDisplayMeshToolTabTitle", "nDisplayMeshTool"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FnDisplayMeshToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FnDisplayMeshToolStyle::Shutdown();

	FnDisplayMeshToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(nDisplayMeshToolTabName);
}

TSharedRef<SDockTab> FnDisplayMeshToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FnDisplayMeshToolModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("nDisplayMeshTool.cpp"))
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

void FnDisplayMeshToolModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(nDisplayMeshToolTabName);
}

void FnDisplayMeshToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FnDisplayMeshToolCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FnDisplayMeshToolCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FnDisplayMeshToolModule, nDisplayMeshTool)