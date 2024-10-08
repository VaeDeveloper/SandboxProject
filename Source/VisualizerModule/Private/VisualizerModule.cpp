#include "VisualizerModule.h"
#include "UnrealEd.h"
#include "SceneComponentVisualizer.h"
#include "DelegateModule/Public/Components/DelegateComponent.h"
#include "SandboxProject/Components/VisualizeSceneComponent.h"

DEFINE_LOG_CATEGORY(VisualizerModule);

#define LOCTEXT_NAMESPACE "FVisualizerModule"

void FVisualizerModule::StartupModule()
{
	UE_LOG(VisualizerModule, Warning, TEXT("VisualizerModule module has been loaded"));
	if (GUnrealEd)
	{
		GUnrealEd->RegisterComponentVisualizer(UVisualizeSceneComponent::StaticClass()->GetFName(), MakeShareable(new SceneComponentVisualizer));
	}
}

void FVisualizerModule::ShutdownModule()
{
	UE_LOG(VisualizerModule, Warning, TEXT("VisualizerModule module has been unloaded"));

	if (GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(UVisualizeSceneComponent::StaticClass()->GetFName());
	}

}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVisualizerModule, VisualizerModule)