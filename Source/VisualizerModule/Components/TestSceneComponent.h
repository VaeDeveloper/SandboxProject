// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TestSceneComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, Pri) )
class VISUALIZERMODULE_API UTestSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UTestSceneComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
