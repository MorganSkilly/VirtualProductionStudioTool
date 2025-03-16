// Copyright Morgan Skillicorn. All Rights Reserved.


#include "nDisplayCurvedWall.h"

#include "ProceduralMeshComponent.h"
#include "ProceduralMeshConversion.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"

#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/StructOnScope.h"

// Constructor
UnDisplayCurvedWall::UnDisplayCurvedWall(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ScreenMesh(TEXT("/nDisplay/Meshes/plane_hd_1x1"));
    SetStaticMesh(ScreenMesh.Object);

    SetMobility(EComponentMobility::Movable);
    SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetVisibility(false);
    SetHiddenInGame(true);
    SetCastShadow(false);

#if WITH_EDITOR
    if (GIsEditor)
    {
        SetVisibility(true);
    }
#endif

    bVisibleInReflectionCaptures = false;
    bVisibleInRayTracing = false;
    bVisibleInRealTimeSkyCaptures = false;

    // Defaults
    SetCurvedWallArraySize(FVector2D(25.f, 9.f));
    SetCurvedWallProductSize(FVector2D(600.f, 337.5f));
    SetCurvedWallProductResolution(FVector2D(384.f, 216.f));
}

FVector2D UnDisplayCurvedWall::GetCurvedWallArraySize() const
{
    const FVector ComponentScale = GetRelativeScale3D();
    const FVector2D ComponentScale2D(ComponentScale.Y, ComponentScale.Z);
    return ComponentScale2D;
}

void UnDisplayCurvedWall::SetCurvedWallArraySize(const FVector2D& ArraySize)
{
    //SetRelativeScale3D(FVector(1.f, InSize.X, InSize.Y));

#if WITH_EDITOR
    //Size = InSize;
#endif
}

FVector2D UnDisplayCurvedWall::GetCurvedWallProductSize() const
{
    const FVector ComponentScale = GetRelativeScale3D();
    const FVector2D ComponentScale2D(ComponentScale.Y, ComponentScale.Z);
    return ComponentScale2D;
}

void UnDisplayCurvedWall::SetCurvedWallProductSize(const FVector2D& ProductSize)
{
    //SetRelativeScale3D(FVector(1.f, InSize.X, InSize.Y));

#if WITH_EDITOR
    //Size = InSize;
#endif
}

FVector2D UnDisplayCurvedWall::GetCurvedWallProductResolution() const
{
    const FVector ComponentScale = GetRelativeScale3D();
    const FVector2D ComponentScale2D(ComponentScale.Y, ComponentScale.Z);
    return ComponentScale2D;
}

void UnDisplayCurvedWall::SetCurvedWallProductResolution(const FVector2D& ProductResolution)
{
    //SetRelativeScale3D(FVector(1.f, InSize.X, InSize.Y));

#if WITH_EDITOR
    //Size = InSize;
#endif
}