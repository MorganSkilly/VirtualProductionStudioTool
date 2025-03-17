// Copyright Morgan Skillicorn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LedProduct.generated.h"

/**
 * ULedProduct - A Data Asset class for defining LED Panel configurations to generate nDisplay volumes procedurally.
 * This is meant to be created and edited in the Content Browser to store panel specifications.
 */
UCLASS(BlueprintType)
class NDISPLAYMESHTOOL_API ULedProduct : public UDataAsset
{
	GENERATED_BODY()

public:
    // Constructor (Sets default values)
    ULedProduct();

    /** Name of the LED panel */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
    FString ModelName;

    /** Physical size of the LED panel in cm (X = width, Y = height, Z = depth) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
    FVector CabinetSize;

    /** Resolution width (pixels) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
    int32 CabinetResolutionX;

    /** Resolution height (pixels) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
    int32 CabinetResolutionY;

    /** Pixel Pitch in mm */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
    float PixelPitch;

    /** The material used for rendering the LED panel */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LED Panel")
    UMaterialInterface* PanelMaterial;

    /** Returns the aspect ratio of the panel (width/height) */
    UFUNCTION(BlueprintCallable, Category = "LED Panel")
    float GetAspectRatio() const;

    /** Prints debug information about the panel (Logs and shows message box in editor mode) */
    UFUNCTION(BlueprintCallable, Category = "LED Panel")
    void PrintPanelInfo() const;
};
