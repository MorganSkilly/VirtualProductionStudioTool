// Copyright Morgan Skillicorn. All Rights Reserved.


#include "LedProduct.h"

#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"
#include "Misc/MessageDialog.h"

// Constructor to initialize default values
ULedProduct::ULedProduct()
{
    ModelName = TEXT("Default LED Panel");
    CabinetSize = FVector(600.0f, 337.5f, 5.0f);  // Default size in cm
    CabinetResolutionX = 384;  // Default Full HD width
    CabinetResolutionY = 216;  // Default Full HD height
    PixelPitch = 1.5625;
    PanelMaterial = nullptr;  // No material by default
}

// Function to get the aspect ratio of the panel
float ULedProduct::GetAspectRatio() const
{
    if (CabinetResolutionY == 0)
    {
        return 0.0f;  // Avoid division by zero
    }
    return static_cast<float>(CabinetResolutionX) / static_cast<float>(CabinetResolutionY);
}

// Function to display panel info in the UE log
void ULedProduct::PrintPanelInfo() const
{
    FString Info = FString::Printf(TEXT("LED Panel: %s\nSize: %s cm\nResolution: %dx%d\nAspect Ratio: %.2f"),
        *ModelName, *CabinetSize.ToString(), CabinetResolutionX, CabinetResolutionY, GetAspectRatio());

    UE_LOG(LogTemp, Log, TEXT("%s"), *Info);

    // Optionally, show a message box in editor mode
#if WITH_EDITOR
    FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Info));
#endif
}

#undef LOCTEXT_NAMESPACE