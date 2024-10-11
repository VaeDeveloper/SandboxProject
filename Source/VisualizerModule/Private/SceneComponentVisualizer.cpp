// This is Sandbox Project.

#include "SceneComponentVisualizer.h"
#include "GameFramework/Actor.h"

#include "Components/ActorComponent.h"

namespace
{
	float DrawDepthBias = 2.0f;
}

/**
 * @brief Draws a visualization for the given scene component.
 *
 * This function renders a wireframe sphere around the specified scene component,
 * and if the component has child components, it draws lines connecting the component
 * to its children. The visualization helps to visualize the relationships between the
 * components of an actor.
 *
 * @param Component The UActorComponent to visualize. Expected to be of type USceneComponent.
 * @param View The scene view in which the visualization is drawn.
 * @param PDI The primitive draw interface used to render the shapes.
 */
void SceneComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const USceneComponent* TestComponent = Cast<USceneComponent>(Component);
	if (! TestComponent) return;

	FTransform TM = TestComponent->GetComponentTransform();
	TM.RemoveScaling();

	const float Radius = 150.0f;

	DrawWireSphereAutoSides(PDI, TM, FColor(200, 255, 255), Radius, SDPG_World);

	const AActor* OwnerActor = TestComponent->GetOwner();
	if (! OwnerActor) return;

	TArray<USceneComponent*> ChildComponents;
	OwnerActor->GetComponents(ChildComponents);

	const int CompCount = ChildComponents.Num();
	for (int i = 0; i < CompCount; i++)
	{
		const USceneComponent* pConnectedComponent = ChildComponents[i];
		if (! pConnectedComponent) continue;

		FTransform ConnectedTarget = pConnectedComponent->GetComponentTransform();
		ConnectedTarget.RemoveScaling();

		PDI->DrawLine(TM.GetLocation(), ConnectedTarget.GetLocation(), FColor::Red, SDPG_Foreground, DrawDepthBias);
	}
}
