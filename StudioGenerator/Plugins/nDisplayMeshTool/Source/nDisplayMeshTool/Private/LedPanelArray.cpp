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
		FVector2D panelsArray = FVector2D(ArrayWidth - 1, ArrayHeight - 1);
		FVector2D panelsDimensions = FVector2D(CabinetSize.X, CabinetSize.Y);
		CreateMesh(PanelAngles, panelsArray, panelsDimensions);
	}
}
#endif

void ALedPanelArray::CreateMesh(TArray<float> panelAngles, FVector2D panels, FVector2D panelDimensions)
{
	TArray<FVector> vertices;
	TArray<int32> Triangles;

	float cumulative_angle = 0.0;

	FVector vectorPos = { 0, 0, 0 };

	for (int32 i = 0; i < panels.X; i++)
	{
		float nextAngleRad = FMath::DegreesToRadians(panelAngles[i]);
		cumulative_angle = cumulative_angle + nextAngleRad;
		FVector nextPoint;
		nextPoint.X = vectorPos.X + panelDimensions.X * FMath::Cos(cumulative_angle);
		nextPoint.Y = vectorPos.Y + panelDimensions.X * FMath::Sin(cumulative_angle);
		nextPoint.Z = vectorPos.Z;

		for (int32 j = 0; j < panels.Y; j++)
		{
			vectorPos = nextPoint;

			vectorPos.Z = j * panelDimensions.Y;

			vertices.Add(vectorPos);
		}
	}

	for (int32 i = 0; i < panels.X - 1; i++)
	{
		for (int32 j = 0; j < panels.Y - 1; j++)
		{
			int32 topLeft = j + (i * panels.Y);
			int32 topRight = topLeft + 1;
			int32 bottomLeft = topLeft + panels.Y;
			int32 bottomRight = bottomLeft + 1;

			Triangles.Add(topLeft);
			Triangles.Add(bottomLeft);
			Triangles.Add(bottomRight);

			Triangles.Add(topLeft);
			Triangles.Add(bottomRight);
			Triangles.Add(topRight);
		}
	}


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

	GeneratedProceduralMeshComponent->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

	//ConvertProcToStatic();
}


void ALedPanelArray::UpdateLedProduct()
{
	ModelName = *LedProductDataAsset->ModelName;
	CabinetSize = LedProductDataAsset->CabinetSize;
	CabinetResolutionX = LedProductDataAsset->CabinetResolutionX;
	CabinetResolutionY = LedProductDataAsset->CabinetResolutionY;
	PixelPitch = LedProductDataAsset->PixelPitch;
	PanelMaterial = LedProductDataAsset->PanelMaterial;
}

