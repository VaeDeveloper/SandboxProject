// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"


/**
 * @brief A visualizer class for scene components.
 *
 * This class extends the FComponentVisualizer and provides custom visualization
 * logic for scene components in the editor. The main responsibility of this class
 * is to render visual aids that help visualize the relationships and positions of
 * components within the scene.
 */
class VISUALIZERMODULE_API SceneComponentVisualizer : public FComponentVisualizer
{
public:
    /**
     * @brief Draws the custom visualization for the scene component.
     *
     * This method overrides the FComponentVisualizer's DrawVisualization method to
     * render a custom visualization for scene components. It is called when the component
     * is selected in the editor, and it uses the given primitive draw interface (PDI)
     * to draw wireframes, lines, or other shapes in the scene.
     *
     * @param Component The UActorComponent being visualized. Expected to be of type USceneComponent.
     * @param View The scene view in which the visualization is drawn.
     * @param PDI The primitive draw interface used for rendering visual aids.
     */
    virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
