// Copyright Morgan Skillicorn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include <vector>

#include "nDisplayCurvedWall.generated.h"

/**
 * Procedurally generated static mesh projection policy screen component
 */
UCLASS()
class NDISPLAYMESHTOOL_API UnDisplayCurvedWall
	: public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UnDisplayCurvedWall();
};
