// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeStudioGenerator_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_StudioGenerator;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_StudioGenerator()
	{
		if (!Z_Registration_Info_UPackage__Script_StudioGenerator.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/StudioGenerator",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x380D19B1,
				0x32BAD34E,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_StudioGenerator.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_StudioGenerator.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_StudioGenerator(Z_Construct_UPackage__Script_StudioGenerator, TEXT("/Script/StudioGenerator"), Z_Registration_Info_UPackage__Script_StudioGenerator, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x380D19B1, 0x32BAD34E));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
