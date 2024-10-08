// This is Sandbox Project. 


#include "SceneComponentVisualizer.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"


void SceneComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const USceneComponent* TestComponent = Cast<USceneComponent>(Component);
	if (!TestComponent) return;

	FTransform TM = TestComponent->GetComponentTransform();
	TM.RemoveScaling();

	const float Radius = 150.0f;

	DrawWireSphereAutoSides(PDI, TM, FColor(200, 255, 255), Radius, SDPG_World);

	const AActor* OwnerActor = TestComponent->GetOwner();
	if (!OwnerActor) return;

	TArray<USceneComponent*> ChildComponents;
	OwnerActor->GetComponents(ChildComponents);

	const int CompCount = ChildComponents.Num();
	for (int i = 0; i < CompCount; i++)
	{
		const USceneComponent* pConnectedComponent = ChildComponents[i];
		if (!pConnectedComponent) continue;

		FTransform ConnectedTarget = pConnectedComponent->GetComponentTransform();
		ConnectedTarget.RemoveScaling();

		PDI->DrawLine(TM.GetLocation(),
			ConnectedTarget.GetLocation(),
			FColor(255, 0, 0),
			SDPG_Foreground,
			2.0f
		);

	}
}
