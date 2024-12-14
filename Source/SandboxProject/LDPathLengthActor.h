// This is Sandbox Project.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"
#include "LDPathLengthActor.generated.h"

class USplineMeshComponent;

/**
 *	Enum representing the world value types for length measurement.
 */
UENUM(BlueprintType)
enum ELengthWolrdValue : uint8
{
	/** Measurement in meters */
	ELWV_Meters UMETA(DisplayName = "Meters"),

	/** Measurement in centimeters */
	ELWV_Cent UMETA(DisplayName = "Centimeters"),
};

/**
 *  AActor subclass that represents a spline with text-based length visulaization spline mesh component
 */
UCLASS(meta = (PrioritizeCategories = "Settings"))
class SANDBOXPROJECT_API ALDPathLengthActor : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * Default constructor. Initializes components and sets default values.
	 * @param ObjectInitializer - Provides initialization parameters for the actor.
	 */
	ALDPathLengthActor(const FObjectInitializer& ObjectInitializer);

	/**
	 * Called when an actor is constructed in the editor.
	 * @param Transform - The transform of the actor.
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR

	// virtual void PostEditUndo() override;
	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
#endif

protected:
	/** The spline component that defines the path. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<USplineComponent> SplineComp;

	/** The text render component that displays the spline's length. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UTextRenderComponent> TextRender;

	/** Enum value representing the unit of length (meters or centimeters). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TEnumAsByte<ELengthWolrdValue> WorldValue = ELengthWolrdValue::ELWV_Meters;

	/** Size of the text in the TextRender component. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "50", ClampMax = "500"))
	float TextRenderSize = 250.0f;

	/** spline mesh scale value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "0.06", ClampMax = "2"))
	float SplineMeshScale = 0.3f;

	/** Color used in the material applied to the spline mesh. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "50", ClampMax = "500"))
	FLinearColor MaterialColor;

	/** Base material instance used for dynamic material creation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "50", ClampMax = "500"))
	TObjectPtr<UMaterialInterface> MaterialInstance;

	/** Name of the material parameter to be set dynamically. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "50", ClampMax = "500"))
	FName MaterialParameterName = FName("");

	/** Static mesh used for spline mesh components. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (ClampMin = "50", ClampMax = "500"))
	TObjectPtr<UStaticMesh> SplineMeshForComponent;

private:
	//UPROPERTY()
	//TObjectPtr<USplineMeshComponent> SplineMeshComponent;

	/**
	 * Updates the text render component with the current spline length based on the selected unit.
	 */
	void SetTextParams();

	/**
	 * Constructs spline mesh components along the spline.
	 */
	void ConstructSplineMeshComponent(const FTransform& SplineTransform);

	void SetInstanceMaterialAndParams(USplineMeshComponent* SplineMesh);

	/** Array holding references to dynamically created spline mesh components. */
	UPROPERTY()
	TArray<TObjectPtr<USplineMeshComponent>> SplineMeshComponents;

	/** Cached spline length in Unreal units (centimeters). */
	float SplineLength = 0.0f;
};
