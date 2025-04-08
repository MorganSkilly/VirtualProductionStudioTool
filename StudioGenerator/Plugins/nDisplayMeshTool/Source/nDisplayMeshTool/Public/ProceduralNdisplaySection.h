// Copyright Morgan Skillicorn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ProceduralNdisplaySection.generated.h"

UCLASS()
class NDISPLAYMESHTOOL_API AProceduralNdisplaySection : public AActor
{
	GENERATED_BODY()

public:
	AProceduralNdisplaySection();

	// Called whenever the actor is constructed or properties are changed
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Procedural mesh generation entry point
	UFUNCTION(CallInEditor, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	void GenerateProceduralMesh();

protected:

	/** Mesh Component that holds the generated geometry */
	UPROPERTY(VisibleAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	UProceduralMeshComponent* MeshComponent;

	/** Number of tiles along the X axis */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	int32 NumX = 10;

	/** Number of tiles along the Y axis */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	int32 NumY = 10;

	/** Size of each tile in Unreal units */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	float TileSize = 100.0f;

	/** Whether to create collision for the mesh */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	bool bCreateCollision = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
	int32 ArrayWidth = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
	int32 ArrayHeight = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
	TArray<float> PanelAngles = TArray<float>({ 10.0f, 10.0f, 10.0f, 10.0f });

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
	ULedProduct* LedProductDataAsset;

	/** Name of the LED panel */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LED Panel")
	FString ModelName = "modelName";

	/** Physical size of the LED panel in cm (X = width, Y = height, Z = depth) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LED Panel")
	FVector CabinetSize;

	/** Resolution width (pixels) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LED Panel")
	int32 CabinetResolutionX;

	/** Resolution height (pixels) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LED Panel")
	int32 CabinetResolutionY;

	/** Pixel Pitch in mm */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LED Panel")
	float PixelPitch;

	/** The material used for rendering the LED panel */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LED Panel")
	UMaterialInterface* PanelMaterial;
	
private:

	// Internal logic to construct the quad mesh
	void BuildMesh();
};
