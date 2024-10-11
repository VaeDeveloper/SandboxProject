// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VisualizeSceneComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SANDBOXPROJECT_API UVisualizeSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UVisualizeSceneComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
