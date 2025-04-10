// Copyright Morgan Skillicorn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "LedProduct.h"
#include "Engine/DataAsset.h"
#include "ProceduralMeshConversion.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Kismet/GameplayStatics.h"

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
	
	/** Bool to enable automatic mesh generation on value change */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	bool AutoUpdate = false;
	
	/** Function callable from the editor to generate the procedural mesh component */
	UFUNCTION(CallInEditor, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	void GenerateProceduralMesh();
	
	/** Function callable from the editor to generate the static mesh component */
	UFUNCTION(CallInEditor, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 1))
	void ConvertToStaticMesh();

protected:
	
	/** Mesh Component that holds the generated procedural geometry */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	AProceduralNdisplaySection* SectionToSnapTo;
	
	/** Mesh Component that holds the generated procedural geometry */
	UPROPERTY(VisibleAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	UProceduralMeshComponent* ProceduralMeshComponent;

	/** Volume/Wall Array Width */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	int32 ArrayWidth = 4;

	/** Volume/Wall Array Height */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	int32 ArrayHeight = 4;

	/** Angular offset between each panel */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (ClampMin = "-180.0", ClampMax = "180.0", DisplayPriority = 0))
	TArray<float> PanelAngles = TArray<float>();

	/** Preset data asset for LED product */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	ULedProduct* LedProductDataAsset;

	/** Angular offset between start and origin */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (ClampMin = "-180.0", ClampMax = "180.0", DisplayPriority = 0))
	float StartingAngle = 0.0f;
	
	/** Starting offset from origin */
	UPROPERTY(EditAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	FVector StartingPos = { 0, 0, 0 };


	/** Cumulative angle of all angular offsets */
	UPROPERTY(VisibleAnywhere, Category = "nDisplay Mesh Tool", meta = (ClampMin = "-180.0", ClampMax = "180.0", DisplayPriority = 0))
	float CumulativeAngle = 0.0f;
	
	/** Position of final point to begin next section */
	UPROPERTY(VisibleAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	FVector EndingPos = { 0, 0, 0 };
	
	/** Name of the LED panel */
	UPROPERTY(VisibleAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	FString ModelName = "modelName";

	/** Physical size of the LED panel in cm (X = width, Y = height, Z = depth) */
	UPROPERTY(VisibleAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	FVector CabinetSize;

	/** Resolution width (pixels) */
	UPROPERTY(VisibleAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	int32 CabinetResolutionX;

	/** Resolution height (pixels) */
	UPROPERTY(VisibleAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	int32 CabinetResolutionY;

	/** Pixel Pitch in mm */
	UPROPERTY(VisibleAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	float PixelPitch;

	/** The material used for rendering the LED panel */
	UPROPERTY(VisibleAnywhere, Category = "nDisplay Mesh Tool", meta = (DisplayPriority = 0))
	UMaterialInterface* PanelMaterial;
	
private:

	void BuildMesh();
	virtual void UpdateLedProduct();
	virtual void ConvertProcToStatic();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	virtual void CreateMesh(TArray<float> panelAngles, FVector2D panels, FVector2D panelDimensions, float startingAngle, FVector startingPos);	
};
