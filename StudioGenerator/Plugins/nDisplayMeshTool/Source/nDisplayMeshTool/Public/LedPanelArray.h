// Copyright Morgan Skillicorn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshConversion.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Engine/DataAsset.h"
#include "LedProduct.h"
#include "DrawDebugHelpers.h"

#include "LedPanelArray.generated.h"

UCLASS()
class NDISPLAYMESHTOOL_API ALedPanelArray : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALedPanelArray();

protected:
	// Called when the game starts or when spawned
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
	virtual void BeginPlay() override;
	virtual void CreateMesh(TArray<float> panelAngles, FVector2D panels, FVector2D panelDimensions);
	virtual void UpdateLedProduct();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
	int32 ArrayWidth = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
	int32 ArrayHeight = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
	TArray<float> PanelAngles = TArray<float>({ 10.0f, 10.0f, 10.0f, 10.0f});

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UProceduralMeshComponent* GeneratedProceduralMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* GeneratedStaticMeshComponent;

private:
	virtual void ConvertProcToStatic();
	/*
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UProceduralMeshComponent* GeneratedProceduralMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* GeneratedStaticMeshComponent;*/

};
