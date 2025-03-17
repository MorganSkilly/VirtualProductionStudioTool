// Copyright Morgan Skillicorn. All Rights Reserved.


#include "nDisplayCurvedWall.h"

#include "ProceduralMeshConversion.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"

#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/StructOnScope.h"

// Constructor
UnDisplayCurvedWall::UnDisplayCurvedWall()
{

#if WITH_EDITOR
    if (GIsEditor)
    {
        SetVisibility(true);
    }
#endif
}