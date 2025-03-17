// Copyright Morgan Skillicorn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshConversion.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"

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
	virtual void CreateMesh();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere, Category = "LED Product")
	//UDataAsset* LedProductDataAsset;

private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UProceduralMeshComponent* GeneratedProceduralMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* GeneratedStaticMeshComponent;

};
