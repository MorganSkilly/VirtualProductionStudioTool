// Copyright Morgan Skillicorn. All Rights Reserved.

#include "ProceduralNdisplaySection.h"
#include "Components/NativeWidgetHost.h"

AProceduralNdisplaySection::AProceduralNdisplaySection()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = ProceduralMeshComponent;
	ProceduralMeshComponent->bUseComplexAsSimpleCollision = true;
}

// Called when the actor is constructed or a property is changed in the editor
void AProceduralNdisplaySection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Callable from editor to generate the procedural mesh
void AProceduralNdisplaySection::GenerateProceduralMesh()
{	
	FVector2D panelsArray = FVector2D(ArrayWidth, ArrayHeight);
	FVector2D panelsDimensions = FVector2D(CabinetSize.X, CabinetSize.Y);

	if (SectionToSnapTo != nullptr)
	{
		CreateMesh(PanelAngles, panelsArray, panelsDimensions, SectionToSnapTo->CumulativeAngle, SectionToSnapTo->EndingPos);
	}
	else
	{		
		CreateMesh(PanelAngles, panelsArray, panelsDimensions, StartingAngle, StartingPos);
	}
}

// Callable from editor to generate the static mesh
void AProceduralNdisplaySection::ConvertToStaticMesh()
{
	ConvertProcToStatic();
}

void AProceduralNdisplaySection::CreateMesh(TArray<float> panelAngles, FVector2D panels, FVector2D panelDimensions, float startingAngle, FVector startingPos)
{
    StartingAngle = startingAngle;

    TArray<FVector> vertices;
    TArray<int32> triangles;
    TArray<FVector2D> UV0;

    float cumulativeAngleDeg = StartingAngle;

    FVector vectorPos = startingPos;
    FVector nextPoint = vectorPos;

    // Generate vertices
    for (int32 i = 0; i <= panels.X; i++)
    {
        if (i != 0 && panelAngles.IsValidIndex(i - 1))
        {
            cumulativeAngleDeg += panelAngles[i - 1];
        }

        for (int32 j = 0; j <= panels.Y; j++)
        {
            vectorPos = nextPoint;
            vectorPos.Z = j * panelDimensions.Y;

            vertices.Add(vectorPos);

            float U = static_cast<float>(i) / panels.X;
            float V = static_cast<float>(j) / panels.Y;

            UV0.Add(FVector2D(U, 1 - V));
        }

        float angleRad = FMath::DegreesToRadians(cumulativeAngleDeg);

        nextPoint.X = vectorPos.X + panelDimensions.X * FMath::Cos(angleRad);
        nextPoint.Y = vectorPos.Y + panelDimensions.X * FMath::Sin(angleRad);
        nextPoint.Z = vectorPos.Z;
    }

    // Generate triangles
    for (int32 i = 0; i < panels.X; i++)
    {
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
    }

    CumulativeAngle = cumulativeAngleDeg;
    EndingPos = vectorPos;

    ProceduralMeshComponent->CreateMeshSection(0, vertices, triangles, {}, UV0, {}, {}, false);
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

    // Copy materials - currently not implemented due to issue with transient materials
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

    bool bSaved = UPackage::SavePackage(Package, StaticMesh, *PackageFileName, SaveArgs);

    if (bSaved)
    {
        // Notify asset registry
        FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
        AssetRegistryModule.AssetCreated(StaticMesh);
        UE_LOG(LogTemp, Log, TEXT("Successfully created and saved static mesh at %s"), *PackageFileName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save package!"));
    }
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
	
	if (PropertyChangedEvent.Property)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProceduralNdisplaySection::StaticClass(), FoundActors);

		for (AActor* Actor : FoundActors)
		{
			if (AProceduralNdisplaySection* Section = Cast<AProceduralNdisplaySection>(Actor))
			{
				if (Section->AutoUpdate)
				{				
					Section->GenerateProceduralMesh();
				}
			}
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
