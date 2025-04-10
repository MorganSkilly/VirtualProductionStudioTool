// Copyright Morgan Skillicorn. All Rights Reserved.

#include "ProceduralNdisplaySection.h"

#include "Components/NativeWidgetHost.h"

// Sets default values
AProceduralNdisplaySection::AProceduralNdisplaySection()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create and configure the Procedural Mesh Component
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = ProceduralMeshComponent;
	ProceduralMeshComponent->bUseComplexAsSimpleCollision = true;
}

// Called when the actor is constructed or a property is changed in the editor
void AProceduralNdisplaySection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// Optionally call GenerateProceduralMesh here if you want auto-regeneration
	// GenerateProceduralMesh();
}

/*// Generates a quad grid based on NumX, NumY, and TileSize
void AProceduralNdisplaySection::BuildMesh()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	const int32 VertexCountX = NumX + 1;
	const int32 VertexCountY = NumY + 1;

	// Generate vertices
	for (int32 y = 0; y < VertexCountY; ++y)
	{
		for (int32 x = 0; x < VertexCountX; ++x)
		{
			Vertices.Add(FVector(x * TileSize, y * TileSize, 0));
			UVs.Add(FVector2D((float)x / NumX, (float)y / NumY));
			Normals.Add(FVector(0, 0, 1));
			VertexColors.Add(FLinearColor::White);
			Tangents.Add(FProcMeshTangent(1, 0, 0));
		}
	}

	// Generate triangles
	for (int32 y = 0; y < NumY; ++y)
	{
		for (int32 x = 0; x < NumX; ++x)
		{
			int32 Index = y * VertexCountX + x;

			// First triangle
			Triangles.Add(Index);
			Triangles.Add(Index + VertexCountX);
			Triangles.Add(Index + 1);

			// Second triangle
			Triangles.Add(Index + 1);
			Triangles.Add(Index + VertexCountX);
			Triangles.Add(Index + VertexCountX + 1);
		}
	}

	// Create the mesh section
	MeshComponent->CreateMeshSection_LinearColor(
		0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, bCreateCollision
	);
}*/

// Callable from editor to generate the procedural mesh
void AProceduralNdisplaySection::GenerateProceduralMesh()
{	
	FVector2D panelsArray = FVector2D(ArrayWidth, ArrayHeight);
	FVector2D panelsDimensions = FVector2D(CabinetSize.X, CabinetSize.Y);

	if (SectionToSnapTo != nullptr)
	{
		CreateMesh(PanelAngles, panelsArray, panelsDimensions, SectionToSnapTo->CumulativeAngle - FMath::DegreesToRadians(SectionToSnapTo->PanelAngles.Last()), SectionToSnapTo->EndingPos);
	}
	else
	{		
		CreateMesh(PanelAngles, panelsArray, panelsDimensions, FMath::DegreesToRadians(StartingAngle), StartingPos);
	}
	//BuildMesh();
}

// Callable from editor to generate the static mesh
void AProceduralNdisplaySection::ConvertToStaticMesh()
{
	ConvertProcToStatic();
}

void AProceduralNdisplaySection::CreateMesh(TArray<float> panelAngles, FVector2D panels, FVector2D panelDimensions, float startingAngle, FVector startingPos)
{
    TArray<FVector> vertices;
    TArray<int32> triangles;
    TArray<FVector2D> UV0;

    float cumulative_angle = startingAngle;

    FVector vectorPos = startingPos;
    FVector nextPoint = vectorPos;
    
    for (int32 i = 0; i <= panels.X; i++)
    {
        float nextAngleRad;

        if (i != 0 && panelAngles.IsValidIndex(i - 1))
        {
            nextAngleRad = FMath::DegreesToRadians(panelAngles[i - 1]);
        }
        else
        {
            nextAngleRad = FMath::DegreesToRadians(0);
        }

        cumulative_angle = cumulative_angle + nextAngleRad;

        for (int32 j = 0; j <= panels.Y; j++)
        {
            vectorPos = nextPoint;
            vectorPos.Z = j * panelDimensions.Y;

            vertices.Add(vectorPos);

            // Compute UV coordinates
            float U = static_cast<float>(i) / panels.X;
            float V = static_cast<float>(j) / panels.Y;

            UV0.Add(FVector2D(U, 1 - V));
        }

        nextPoint.X = vectorPos.X + panelDimensions.X * FMath::Cos(cumulative_angle);
        nextPoint.Y = vectorPos.Y + panelDimensions.X * FMath::Sin(cumulative_angle);
        nextPoint.Z = vectorPos.Z;
    }

    for (int32 i = 0; i < panels.X; i++)
    {
        float nextAngleRad;

        if (i != 0 && panelAngles.IsValidIndex(i - 1))
        {
            nextAngleRad = FMath::DegreesToRadians(panelAngles[i - 1]);
        }
        else
        {
            nextAngleRad = FMath::DegreesToRadians(0);
        }

        cumulative_angle += nextAngleRad;

        for (int32 j = 0; j < panels.Y; j++)
        {
            int32 topLeft = i * (panels.Y + 1) + j;
            int32 topRight = (i + 1) * (panels.Y + 1) + j;
            int32 bottomLeft = i * (panels.Y + 1) + (j + 1);
            int32 bottomRight = (i + 1) * (panels.Y + 1) + (j + 1);

            triangles.Add(topLeft);
            triangles.Add(topRight);
            triangles.Add(bottomLeft);

            triangles.Add(topRight);
            triangles.Add(bottomRight);
            triangles.Add(bottomLeft);
        }

        nextPoint.X = vectorPos.X + panelDimensions.X * FMath::Cos(cumulative_angle);
        nextPoint.Y = vectorPos.Y + panelDimensions.X * FMath::Sin(cumulative_angle);
        nextPoint.Z = vectorPos.Z;
    }

	CumulativeAngle = cumulative_angle;
	EndingPos = vectorPos;
    ProceduralMeshComponent->CreateMeshSection(0, vertices, triangles, {}, UV0, {}, {}, false);

    //ConvertProcToStatic();
}

void AProceduralNdisplaySection::ConvertProcToStatic()
{
    UProceduralMeshComponent* ProcMeshComp = ProceduralMeshComponent;
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
    FString NewNameSuggestion = ModelName + FString::FromInt(ArrayWidth) + "x" + FString::FromInt(ArrayHeight) + "_" + FString::FromInt(GetUniqueID());
    FString ContentDir = FPaths::ProjectContentDir() / TEXT("GeneratedMeshes");

    // Create directory if it doesn't exist
    IFileManager::Get().MakeDirectory(*ContentDir, true);

    FString PackageName = TEXT("/Game/nDisplayMeshTool/") + NewNameSuggestion;
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
    SrcModel.BuildSettings.bRecomputeNormals = true;
    SrcModel.BuildSettings.bRecomputeTangents = true;
    SrcModel.BuildSettings.bRemoveDegenerates = true;
    SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
    SrcModel.BuildSettings.bUseFullPrecisionUVs = true;
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
        /*UMaterialInterface* Material = ProcMeshComp->GetMaterial(SectionIdx);
        if (Material)
        {
            StaticMesh->GetStaticMaterials().Add(FStaticMaterial(Material));
        }*/
    }

    // Set import version and build
    StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;
    StaticMesh->Build(false);
    StaticMesh->PostEditChange();

    // Mark package as dirty
    Package->MarkPackageDirty();

    // Save the package
    FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

    // Create SavePackageArgs
    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
    SaveArgs.SaveFlags = SAVE_NoError;

    //bool bSaved = UPackage::SavePackage(Package, StaticMesh, *PackageFileName, SaveArgs);

    //if (bSaved)
    //{
    //    // Now notify asset registry
    //    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    //    AssetRegistryModule.AssetCreated(StaticMesh);
    //    UE_LOG(LogTemp, Log, TEXT("Successfully created and saved static mesh at %s"), *PackageFileName);
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Error, TEXT("Failed to save package!"));
    //}

    //StaticMeshComponent->SetStaticMesh(StaticMesh);
}

#if WITH_EDITOR
void AProceduralNdisplaySection::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property &&
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AProceduralNdisplaySection, LedProductDataAsset))
	{
		UpdateLedProduct();
	}
	
	if (PropertyChangedEvent.Property &&
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AProceduralNdisplaySection, ArrayWidth))
	{
		if (PanelAngles.Num() != ArrayWidth)
		{
			PanelAngles.SetNum(ArrayWidth);
		}
	}
	
	if (AutoUpdate)
	{
		if (PropertyChangedEvent.Property)
		{
			GenerateProceduralMesh();
		}		
	}
}
#endif

void AProceduralNdisplaySection::UpdateLedProduct()
{
	if (LedProductDataAsset)
	{
		if (*LedProductDataAsset->ModelName)
		{
			ModelName = *LedProductDataAsset->ModelName;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ModelName is nullptr in LedProductDataAsset"));
		}

		CabinetSize = LedProductDataAsset->CabinetSize;
		CabinetResolutionX = LedProductDataAsset->CabinetResolutionX;
		CabinetResolutionY = LedProductDataAsset->CabinetResolutionY;
		PixelPitch = LedProductDataAsset->PixelPitch;
		PanelMaterial = LedProductDataAsset->PanelMaterial;

		UMaterialInstanceDynamic* materialInstance = ProceduralMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, PanelMaterial);

		materialInstance->SetScalarParameterValue("Width", ArrayWidth * LedProductDataAsset->CabinetResolutionX);
		materialInstance->SetScalarParameterValue("Height", ArrayHeight * LedProductDataAsset->CabinetResolutionY);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LedProductDataAsset is nullptr!"));
	}
}

// Called when the game starts or when spawned
void AProceduralNdisplaySection::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame (currently disabled)
void AProceduralNdisplaySection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
