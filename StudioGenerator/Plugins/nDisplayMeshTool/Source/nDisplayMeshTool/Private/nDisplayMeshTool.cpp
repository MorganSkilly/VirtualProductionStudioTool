// Copyright Morgan Skillicorn. All Rights Reserved.

#include "nDisplayMeshTool.h"
#include "nDisplayMeshToolStyle.h"
#include "nDisplayMeshToolCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"

static const FName nDisplayMeshToolTabName("nDisplayMeshTool");

#define LOCTEXT_NAMESPACE "FnDisplayMeshToolModule"

void FnDisplayMeshToolModule::StartupModule()
{
	// Style and command setup
	FnDisplayMeshToolStyle::Initialize();
	FnDisplayMeshToolStyle::ReloadTextures();

	// Property editor access
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// ✅ Create custom section for ProceduralNdisplaySection (class name without 'A')
	TSharedRef<FPropertySection> Section = PropertyModule.FindOrCreateSection(
		"ProceduralNdisplaySection",
		"GM",
		LOCTEXT("nDisplay Mesh Tool", "nDisplay Mesh Tool")
	);

	Section->AddCategory("nDisplay Mesh Tool");
}

void FnDisplayMeshToolModule::ShutdownModule()
{
	// Unregister details layout or cleanup if needed
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout("ProceduralNdisplaySection");
	}

	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FnDisplayMeshToolStyle::Shutdown();
	FnDisplayMeshToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(nDisplayMeshToolTabName);
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FnDisplayMeshToolModule, nDisplayMeshTool)
