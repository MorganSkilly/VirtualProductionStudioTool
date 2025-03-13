#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralVolumeMesh.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NDISPLAYPROCEDURALVOLUME_API UProceduralVolumeMesh : public UStaticMeshComponent
{
    GENERATED_BODY()

public:
    UProceduralVolumeMesh();

    /** Mesh Generation Parameters */
    UPROPERTY(EditAnywhere, Category = "Mesh Settings")
    float Width = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Mesh Settings")
    float Height = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Mesh Settings")
    float Depth = 100.0f;

    /** Generates the procedural mesh */
    void GenerateMesh();

protected:
    virtual void OnRegister() override;

    // Called when a property is changed in the editor
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
