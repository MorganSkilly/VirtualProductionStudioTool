#include "ProceduralVolumeMesh.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshAttributes.h"
#include "MeshDescription.h"
#include "RenderingThread.h"
#include "StaticMeshOperations.h"
#include "AssetRegistryModule.h"
#include "Editor/UnrealEd/Public/AssetToolsModule.h"

UProceduralVolumeMesh::UProceduralVolumeMesh()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UProceduralVolumeMesh::OnRegister()
{
    Super::OnRegister();
    GenerateMesh();
}

void UProceduralVolumeMesh::GenerateMesh()
{
    // Create a new Static Mesh
    UStaticMesh* NewMesh = NewObject<UStaticMesh>(this, UStaticMesh::StaticClass());

    if (!NewMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create UStaticMesh!"));
        return;
    }

    // Ensure the mesh has a valid description, create if not present
    if (!NewMesh->GetMeshDescription(0))
    {
        NewMesh->CreateMeshDescription(0); // Create mesh description if it doesn't exist
    }

    // Attempt to retrieve the mesh description
    FMeshDescription* MeshDescription = NewMesh->GetMeshDescription(0);
    if (!MeshDescription)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to retrieve MeshDescription!"));
        return;
    }

    // Log to check if MeshDescription is valid
    UE_LOG(LogTemp, Log, TEXT("MeshDescription successfully retrieved."));

    // Clear previous mesh data
    MeshDescription->Empty();

    // Register static mesh attributes
    FStaticMeshAttributes Attributes(*MeshDescription);
    Attributes.Register();
    TVertexAttributesRef<FVector3f> VertexPositions = Attributes.GetVertexPositions();

    // Create Vertex IDs and assign positions
    FVertexID V0 = MeshDescription->CreateVertex();
    VertexPositions[V0] = FVector3f(0, 0, 0);

    FVertexID V1 = MeshDescription->CreateVertex();
    VertexPositions[V1] = FVector3f(Width, 0, 0);

    FVertexID V2 = MeshDescription->CreateVertex();
    VertexPositions[V2] = FVector3f(Width, Height, 0);

    FVertexID V3 = MeshDescription->CreateVertex();
    VertexPositions[V3] = FVector3f(0, Height, 0);

    // Create a Polygon Group
    FPolygonGroupID PolygonGroup = MeshDescription->CreatePolygonGroup();

    // Create two triangles to form a quad
    MeshDescription->CreateTriangle(PolygonGroup, { V0, V1, V2 });
    MeshDescription->CreateTriangle(PolygonGroup, { V2, V3, V0 });

    // Commit mesh description
    NewMesh->CommitMeshDescription(0);

    // Build & finalize the static mesh
    NewMesh->Build(false);
    NewMesh->CreateBodySetup();
    NewMesh->MarkPackageDirty();
    NewMesh->PostEditChange();

    // Assign the new mesh to the component
    SetStaticMesh(NewMesh);
}

void UProceduralVolumeMesh::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Check if a relevant property was changed (e.g., Width, Height, or Depth)
    if (PropertyChangedEvent.Property != nullptr)
    {
        // You can check the exact name of the property here if you want
        const FName PropertyName = PropertyChangedEvent.Property->GetFName();
        if (PropertyName == GET_MEMBER_NAME_CHECKED(UProceduralVolumeMesh, Width) ||
            PropertyName == GET_MEMBER_NAME_CHECKED(UProceduralVolumeMesh, Height) ||
            PropertyName == GET_MEMBER_NAME_CHECKED(UProceduralVolumeMesh, Depth))
        {
            // Regenerate the mesh
            GenerateMesh();
        }
    }
}
