// Copyright Morgan Skillicorn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "TestMesh.generated.h"

UCLASS()
class NDISPLAYMESHTOOL_API ATestMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
	virtual void CreateTriangle();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* mesh;
};
