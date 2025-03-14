// Copyright Morgan Skillicorn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshConversion.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "nDisplayVolume.generated.h"

/**
 * 
 */
UCLASS()
class NDISPLAYMESHTOOL_API AnDisplayVolume : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AnDisplayVolume();
	// Called every frame
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void CreateMesh();

private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UProceduralMeshComponent* ProceduralMesh;
	virtual void ConvertProcToStatic();
};
