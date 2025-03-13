// Copyright Epic Games, Inc. All Rights Reserved.

#include "nDisplayProceduralVolumeStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FnDisplayProceduralVolumeStyle::StyleInstance = nullptr;

void FnDisplayProceduralVolumeStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FnDisplayProceduralVolumeStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FnDisplayProceduralVolumeStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("nDisplayProceduralVolumeStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FnDisplayProceduralVolumeStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("nDisplayProceduralVolumeStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("nDisplayProceduralVolume")->GetBaseDir() / TEXT("Resources"));

	Style->Set("nDisplayProceduralVolume.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FnDisplayProceduralVolumeStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FnDisplayProceduralVolumeStyle::Get()
{
	return *StyleInstance;
}
