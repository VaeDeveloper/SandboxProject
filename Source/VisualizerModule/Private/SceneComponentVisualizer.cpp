// This is Sandbox Project. 


#include "SceneComponentVisualizer.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"


void SceneComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
    // 

    const USceneComponent* TestComponent = Cast<USceneComponent>(Component);
    if (!TestComponent) return;
    
    FTransform TM = TestComponent->GetComponentTransform();
    TM.RemoveScaling();
    
    const float Radius = 150.0f;
    // Рисуем радиус света
    DrawWireSphereAutoSides(PDI, TM, FColor(200, 255, 255), Radius, SDPG_World);
    
    // Получаем актера, владельца компонента
    const AActor* OwnerActor = TestComponent->GetOwner();
    if (!OwnerActor) return;
    
    // Получаем все дочерние компоненты типа USceneComponent
    TArray<USceneComponent*> ChildComponents;
    OwnerActor->GetComponents(ChildComponents);
    
    const int CompCount = ChildComponents.Num();
    for (int i = 0; i < CompCount; i++)
    {
        // Приводим каждый компонент к USceneComponent
        const USceneComponent* pConnectedComponent = ChildComponents[i];
        if (pConnectedComponent)
        {
            FTransform ConnectedTarget = pConnectedComponent->GetComponentTransform();
            ConnectedTarget.RemoveScaling();
    
            // Рисуем линию до каждого дочернего компонента
            PDI->DrawLine(
                TM.GetLocation(),
                ConnectedTarget.GetLocation(),
                FColor(255, 0, 0), // Красный цвет
                SDPG_Foreground,
                2.0f
            );
        }
    }
}
