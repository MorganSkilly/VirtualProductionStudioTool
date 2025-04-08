// Copyright Morgan Skillicorn. All Rights Reserved.

#include "ProceduralNdisplaySection.h"

// Sets default values
AProceduralNdisplaySection::AProceduralNdisplaySection()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create and configure the Procedural Mesh Component
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = MeshComponent;
	MeshComponent->bUseComplexAsSimpleCollision = true;
}

// Called when the actor is constructed or a property is changed in the editor
void AProceduralNdisplaySection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// Optionally call GenerateProceduralMesh here if you want auto-regeneration
	// GenerateProceduralMesh();
}

// Generates a quad grid based on NumX, NumY, and TileSize
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
}

// Callable from editor to generate the procedural mesh
void AProceduralNdisplaySection::GenerateProceduralMesh()
{
	BuildMesh();
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
