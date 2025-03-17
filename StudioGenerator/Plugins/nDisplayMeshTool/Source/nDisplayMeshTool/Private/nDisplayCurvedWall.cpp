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

    ConvertToStatic(CreateMeshProcedurally(ProceduralCurvedWallMesh, { 0.f }));
}

FVector2D UnDisplayCurvedWall::GetCurvedWallArraySize() const
{
    return FVector2D(1, 1);
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
    return FVector2D(1, 1);
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
    return FVector2D(1, 1);
}

void UnDisplayCurvedWall::SetCurvedWallProductResolution(const FVector2D& ProductResolution)
{
    //SetRelativeScale3D(FVector(1.f, InSize.X, InSize.Y));

#if WITH_EDITOR
    //Size = InSize;
#endif
}

UProceduralMeshComponent* UnDisplayCurvedWall::CreateMeshProcedurally(UProceduralMeshComponent* procMesh, const std::vector<float>& offsetAngles)
{
    // If procMesh is null, create a new UProceduralMeshComponent
    if (!procMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("procMesh is null, creating a new Procedural Mesh Component!"));

        // Create a new Procedural Mesh Component
        procMesh = NewObject<UProceduralMeshComponent>(this, UProceduralMeshComponent::StaticClass(), TEXT("ProcMeshComponent"));

        // Register the new component
        procMesh->RegisterComponent();

        // Check if we are within an Actor or Component
        AActor* OwnerActor = GetOwner();  // Get the owner actor of this component
        if (OwnerActor)
        {
            // Attach the procedural mesh component to the actor's root component
            procMesh->SetupAttachment(OwnerActor->GetRootComponent());  // Use GetRootComponent() to access the root
        }

        // Optionally, you can set other properties or initialize things here
    }

    // Log offset angles
    FString AngleStr = "Offset Angles: ";
    for (float val : offsetAngles) {
        AngleStr += FString::Printf(TEXT("%f "), val);
    }
    UE_LOG(LogTemp, Warning, TEXT("%s"), *AngleStr);

    // Clear previous mesh sections if necessary
    procMesh->ClearMeshSection(0);

    // Define vertices, triangles, normals, UVs, tangents, and vertex colors
    TArray<FVector> vertices;
    vertices.Add(FVector(0, 0, 0));
    vertices.Add(FVector(0, 100, 0));
    vertices.Add(FVector(0, 0, 100));

    TArray<int32> Triangles;
    Triangles.Add(0);
    Triangles.Add(1);
    Triangles.Add(2);

    TArray<FVector> normals;
    normals.Add(FVector(1, 0, 0));
    normals.Add(FVector(1, 0, 0));
    normals.Add(FVector(1, 0, 0));

    TArray<FVector2D> UV0;
    UV0.Add(FVector2D(0, 0));
    UV0.Add(FVector2D(10, 0));
    UV0.Add(FVector2D(0, 10));

    TArray<FProcMeshTangent> tangents;
    tangents.Add(FProcMeshTangent(0, 1, 0));
    tangents.Add(FProcMeshTangent(0, 1, 0));
    tangents.Add(FProcMeshTangent(0, 1, 0));

    TArray<FLinearColor> vertexColors;
    vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
    vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
    vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

    // Create the mesh section
    procMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

    // Return the procedural mesh component (either the one passed in or the new one created)
    return procMesh;
}


UStaticMesh* UnDisplayCurvedWall::ConvertToStatic(UProceduralMeshComponent* procMesh)
{
    if (!procMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("CreateMeshProcedurally: procMesh is null!"));
        return nullptr;
    }

    UProceduralMeshComponent* ProcMeshComp = procMesh;
    if (!ProcMeshComp)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid procedural mesh component!"));
        return nullptr;
    }

    // Build mesh description
    FMeshDescription MeshDescription = BuildMeshDescription(ProcMeshComp);

    if (MeshDescription.Polygons().Num() <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("No polygons in mesh description!"));
        return nullptr;
    }

    // Setup paths and names
    FString NewNameSuggestion = FString(TEXT("ConvertedNdisplayMesh"));
    FString ContentDir = FPaths::ProjectContentDir() / TEXT("GeneratedNdisplayMeshes");

    // Create directory if it doesn't exist
    IFileManager::Get().MakeDirectory(*ContentDir, true);

    FString PackageName = TEXT("/Game/GeneratedNdisplayMeshes/") + NewNameSuggestion;
    UPackage* Package = CreatePackage(*PackageName);

    if (!Package)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create package!"));
        return nullptr;
    }

    // Create the static mesh
    UStaticMesh* convertedMesh = NewObject<UStaticMesh>(Package, *NewNameSuggestion, RF_Public | RF_Standalone);
    if (!convertedMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create static mesh!"));
        return nullptr;
    }

    convertedMesh->InitResources();
    convertedMesh->SetLightingGuid();

    // Add source to new StaticMesh
    FStaticMeshSourceModel& SrcModel = convertedMesh->AddSourceModel();
    SrcModel.BuildSettings.bRecomputeNormals = false;
    SrcModel.BuildSettings.bRecomputeTangents = false;
    SrcModel.BuildSettings.bRemoveDegenerates = false;
    SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
    SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
    SrcModel.BuildSettings.bGenerateLightmapUVs = true;
    SrcModel.BuildSettings.SrcLightmapIndex = 0;
    SrcModel.BuildSettings.DstLightmapIndex = 1;

    // Create mesh description and commit it
    convertedMesh->CreateMeshDescription(0, MoveTemp(MeshDescription));
    convertedMesh->CommitMeshDescription(0);

    // Copy materials
    const int32 NumSections = ProcMeshComp->GetNumSections();
    for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
    {
        UMaterialInterface* Material = ProcMeshComp->GetMaterial(SectionIdx);
        if (Material)
        {
            convertedMesh->GetStaticMaterials().Add(FStaticMaterial(Material));
        }
    }

    // Set import version and build
    convertedMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;
    convertedMesh->Build(false);
    convertedMesh->PostEditChange();

    // Mark package as dirty
    Package->MarkPackageDirty();

    // Save the package
    FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

    // Create SavePackageArgs
    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
    SaveArgs.SaveFlags = SAVE_NoError;

    // Use the new SavePackage method
    bool bSaved = UPackage::SavePackage(Package, convertedMesh, *PackageFileName, SaveArgs);

    if (bSaved)
    {
        // Now notify asset registry
        FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
        AssetRegistryModule.AssetCreated(convertedMesh);
        UE_LOG(LogTemp, Log, TEXT("Successfully created and saved static mesh at %s"), *PackageFileName);
        return convertedMesh;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save package!"));
        return nullptr;
    }
}