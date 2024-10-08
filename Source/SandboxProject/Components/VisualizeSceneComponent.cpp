// This is Sandbox Project. 


#include "VisualizeSceneComponent.h"


UVisualizeSceneComponent::UVisualizeSceneComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UVisualizeSceneComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UVisualizeSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

