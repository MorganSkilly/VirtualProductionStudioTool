// Copyright Morgan Skillicorn. All Rights Reserved.


#include "LedPanelArray.h"

// Sets default values
ALedPanelArray::ALedPanelArray()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the procedural mesh component
    GeneratedProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedProceduralMeshComponent"));
    GeneratedProceduralMeshComponent->SetupAttachment(RootComponent);

    GeneratedStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratedStaticMeshComponent"));
    GeneratedStaticMeshComponent->SetupAttachment(RootComponent);

	// Optional settings
	GeneratedProceduralMeshComponent->bUseAsyncCooking = true;
	GeneratedProceduralMeshComponent->bUseComplexAsSimpleCollision = false;
}

// This is called when actor is spawned (at runtime or when you drop it into the world in editor)
void ALedPanelArray::PostActorCreated()
{
	Super::PostActorCreated();

}

// This is called when actor is already in level and map is opened
void ALedPanelArray::PostLoad()
{
	/*FVector2D panelsArray = FVector2D(ArrayWidth - 1, ArrayHeight - 1);
	FVector2D panelsDimensions = FVector2D(CabinetResolutionX, CabinetResolutionY);*/
	Super::PostLoad();
}

// Called when the game starts or when spawned
void ALedPanelArray::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALedPanelArray::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

#if WITH_EDITOR
void ALedPanelArray::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property &&
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ALedPanelArray, LedProductDataAsset))
	{
		UpdateLedProduct();
        FVector2D panelsArray = FVector2D(ArrayWidth, ArrayHeight);
		FVector2D panelsDimensions = FVector2D(CabinetSize.X, CabinetSize.Y);
		CreateMesh(PanelAngles, panelsArray, panelsDimensions);
        ConvertProcToStatic();



        GeneratedProceduralMeshComponent->SetVisibility(false, true);
        GeneratedProceduralMeshComponent->SetHiddenInGame(true);
	}
}
#endif

void ALedPanelArray::CreateMesh(TArray<float> panelAngles, FVector2D panels, FVector2D panelDimensions)
{

	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FVector2D> UV0;

	float cumulative_angle = 0.0;

	FVector vectorPos = { 0, 0, 0 };
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

			UV0.Add(FVector2D(U, V));

			DrawDebugPoint(GetWorld(), vectorPos, 10.f, FColor::Green, false, 10.f, 0);
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

            DrawDebugLine(GetWorld(), vertices[topLeft], vertices[topRight], FColor::Red, false, 10.f, 0, 5.f);
            DrawDebugLine(GetWorld(), vertices[topRight], vertices[bottomLeft], FColor::Red, false, 10.f, 0, 5.f);
            DrawDebugLine(GetWorld(), vertices[bottomLeft], vertices[topLeft], FColor::Red, false, 10.f, 0, 5.f);

            DrawDebugLine(GetWorld(), vertices[topRight], vertices[bottomRight], FColor::Red, false, 10.f, 0, 5.f);
            DrawDebugLine(GetWorld(), vertices[bottomRight], vertices[bottomLeft], FColor::Red, false, 10.f, 0, 5.f);
            DrawDebugLine(GetWorld(), vertices[bottomLeft], vertices[topRight], FColor::Red, false, 10.f, 0, 5.f);
        }

        nextPoint.X = vectorPos.X + panelDimensions.X * FMath::Cos(cumulative_angle);
        nextPoint.Y = vectorPos.Y + panelDimensions.X * FMath::Sin(cumulative_angle);
        nextPoint.Z = vectorPos.Z;
    }

	//TArray<FVector> normals;
	//normals.Add(FVector(1, 0, 0));
	//normals.Add(FVector(1, 0, 0));
	//normals.Add(FVector(1, 0, 0));

	//TArray<FVector2D> UV0;
	//UV0.Add(FVector2D(0, 0));
	//UV0.Add(FVector2D(10, 0));
	//UV0.Add(FVector2D(0, 10));

	//TArray<FProcMeshTangent> tangents;
	//tangents.Add(FProcMeshTangent(0, 1, 0));
	//tangents.Add(FProcMeshTangent(0, 1, 0));
	//tangents.Add(FProcMeshTangent(0, 1, 0));

	//TArray<FLinearColor> vertexColors;
	//vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	//vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	//vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

	GeneratedProceduralMeshComponent->CreateMeshSection(0, vertices, triangles, {}, UV0, {}, {}, false);

	
	//ConvertProcToStatic();
}


void ALedPanelArray::UpdateLedProduct()
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
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LedProductDataAsset is nullptr!"));
	}
}

void ALedPanelArray::ConvertProcToStatic()
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
    FString NewNameSuggestion = ModelName + FString::FromInt(ArrayWidth) + "x" + FString::FromInt(ArrayHeight) + "_" + FString::FromInt(GetUniqueID());
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

    // Create SavePackageArgs
    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
    SaveArgs.SaveFlags = SAVE_NoError;

    // Use the new SavePackage method
    bool bSaved = UPackage::SavePackage(Package, StaticMesh, *PackageFileName, SaveArgs);

    if (bSaved)
    {
        // Now notify asset registry
        FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
        AssetRegistryModule.AssetCreated(StaticMesh);
        UE_LOG(LogTemp, Log, TEXT("Successfully created and saved static mesh at %s"), *PackageFileName);

        GeneratedStaticMeshComponent->SetStaticMesh(StaticMesh);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save package!"));
    }
}
