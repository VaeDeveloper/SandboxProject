// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"


/**
 * 
 */
class VISUALIZERMODULE_API SceneComponentVisualizer : public FComponentVisualizer
{
public:
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
