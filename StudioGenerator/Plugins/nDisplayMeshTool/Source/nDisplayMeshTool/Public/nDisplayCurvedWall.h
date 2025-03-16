// Copyright Morgan Skillicorn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

#include "nDisplayCurvedWall.generated.h"

/**
 * Procedurally generated static mesh projection policy screen component
 */
UCLASS(ClassGroup = (DisplayCluster), meta = (BlueprintSpawnableComponent, DisplayName = "NDisplay Curved Wall"), HideCategories = (StaticMesh, Materials, ComponentTick, Physics, Collision, Lighting, Navigation, VirtualTexture, ComponentReplication, Cooking, LOD, MaterialParameters, HLOD, RayTracing, TextureStreaming, Mobile))
class NDISPLAYMESHTOOL_API UnDisplayCurvedWall
	: public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UnDisplayCurvedWall(const FObjectInitializer& ObjectInitializer);

public:
	/** Return the wall panel array size. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get curved wall array size"), Category = "NDisplay|Components")
	FVector2D GetCurvedWallArraySize() const;

	/** Set wall panel array size. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set curved wall array size"), Category = "NDisplay|Components")
	void SetCurvedWallArraySize(const FVector2D& Size);

	/** Return the wall panel product size. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get curved wall product size"), Category = "NDisplay|Components")
	FVector2D GetCurvedWallProductSize() const;

	/** Set wall panel product size. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set curved wall product size"), Category = "NDisplay|Components")
	void SetCurvedWallProductSize(const FVector2D& Size);

	/** Return the wall panel product resolution. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get curved wall product resolution"), Category = "NDisplay|Components")
	FVector2D GetCurvedWallProductResolution() const;

	/** Set wall panel product resolution. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set curved wall product resolution"), Category = "NDisplay|Components")
	void SetCurvedWallProductResolution(const FVector2D& Size);

#if WITH_EDITOR
protected:
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/** Updates Size vector when scale is changed explicitly */
	//void UpdateScreenSizeFromScale();
#endif

#if WITH_EDITORONLY_DATA
protected:
	//friend class FDisplayClusterConfiguratorScreenDetailsCustomization;

	/** Adjust the size of the screen. */
	UPROPERTY(EditDefaultsOnly, Category = "Curved Wall", meta = (DisplayName = "Curved Wall Array Size", AllowPreserveRatio))
	FVector2D CurvedWallArraySize;

	UPROPERTY(EditDefaultsOnly, Category = "Curved Wall", meta = (DisplayName = "Curved Wall Product Size", AllowPreserveRatio))
	FVector2D CurvedWallProductSize;

	UPROPERTY(EditDefaultsOnly, Category = "Curved Wall", meta = (DisplayName = "Curved Wall Product Resolution", AllowPreserveRatio))
	FVector2D CurvedWallProductResolution;
#endif
};
