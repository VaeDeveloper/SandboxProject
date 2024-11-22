// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"
#include "LDPathLengthActor.generated.h"

UCLASS(meta = (PrioritizeCategories = "Settings"))
class SANDBOXPROJECT_API ALDPathLengthActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALDPathLengthActor();

protected:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<USplineComponent> SplineComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UTextRenderComponent> TextRender;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SplineLength = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool bIsM = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (ClampMin = "50", ClampMax = "500"))
	float TextRenderSize = 50.0f;
	


};
