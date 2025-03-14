// Copyright Morgan Skillicorn. All Rights Reserved.


#include "nDisplayVolume.h"

// Constructor
AnDisplayVolume::AnDisplayVolume()
{
    // Create the procedural mesh component
    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GenratedMesh"));
    ProceduralMesh->SetupAttachment(RootComponent);

    // Optional settings
    ProceduralMesh->bUseAsyncCooking = true;
    ProceduralMesh->bUseComplexAsSimpleCollision = false;
}

// Called every frame
void AnDisplayVolume::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// This is called when actor is spawned (at runtime or when you drop it into the world in editor)
void AnDisplayVolume::PostActorCreated()
{
    Super::PostActorCreated();
    CreateMesh();

}

// This is called when actor is already in level and map is opened
void AnDisplayVolume::PostLoad()
{
    Super::PostLoad();
	//CreateMesh();
}

// Called when the game starts or when spawned
void AnDisplayVolume::BeginPlay()
{
    Super::BeginPlay();

}

void AnDisplayVolume::CreateMesh()
{
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

	ProceduralMesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

	ConvertProcToStatic();
}

void AnDisplayVolume::ConvertProcToStatic()
{
    // Find the procedural mesh component
    TArray<UProceduralMeshComponent*> ChildMeshes;
    this->GetComponents<UProceduralMeshComponent>(ChildMeshes);

    if (ChildMeshes.Num() <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("No procedural mesh component found!"));
        return;
    }

    UProceduralMeshComponent* ProcMeshComp = ChildMeshes[0];
    if (!ProcMeshComp)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid procedural mesh component!"));
        return;
    }

    // Build mesh description
    FMeshDescription MeshDescription = BuildMeshDescription(ProcMeshComp);

    if (MeshDescription.Polygons().Num() <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("No polygons in mesh description!"));
        return;
    }

    // Setup paths and names
    FString NewNameSuggestion = FString(TEXT("ConvertedMesh"));
    FString ContentDir = FPaths::ProjectContentDir() / TEXT("GeneratedMeshes");

    // Create directory if it doesn't exist
    IFileManager::Get().MakeDirectory(*ContentDir, true);

    FString PackageName = TEXT("/Game/GeneratedMeshes/") + NewNameSuggestion;
    UPackage* Package = CreatePackage(*PackageName);

    if (!Package)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create package!"));
        return;
    }

    // Create the static mesh
    UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, *NewNameSuggestion, RF_Public | RF_Standalone);
    if (!StaticMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create static mesh!"));
        return;
    }

    StaticMesh->InitResources();
    StaticMesh->SetLightingGuid();

    // Add source to new StaticMesh
    FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
    SrcModel.BuildSettings.bRecomputeNormals = false;
    SrcModel.BuildSettings.bRecomputeTangents = false;
    SrcModel.BuildSettings.bRemoveDegenerates = false;
    SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
    SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
    SrcModel.BuildSettings.bGenerateLightmapUVs = true;
    SrcModel.BuildSettings.SrcLightmapIndex = 0;
    SrcModel.BuildSettings.DstLightmapIndex = 1;

    // Create mesh description and commit it
    StaticMesh->CreateMeshDescription(0, MoveTemp(MeshDescription));
    StaticMesh->CommitMeshDescription(0);

    // Copy materials
    const int32 NumSections = ProcMeshComp->GetNumSections();
    for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
    {
        UMaterialInterface* Material = ProcMeshComp->GetMaterial(SectionIdx);
        if (Material)
        {
            StaticMesh->GetStaticMaterials().Add(FStaticMaterial(Material));
        }
    }

    // Set import version and build
    StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;
    StaticMesh->Build(false);
    StaticMesh->PostEditChange();

    // Mark package as dirty
    Package->MarkPackageDirty();

    // Save the package
    FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
    bool bSaved = UPackage::SavePackage(Package, StaticMesh, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName);

    if (bSaved)
    {
        // Now notify asset registry
        FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
        AssetRegistryModule.AssetCreated(StaticMesh);
        UE_LOG(LogTemp, Log, TEXT("Successfully created and saved static mesh at %s"), *PackageFileName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save package!"));
    }
}
