// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "ChainComponent.h"
#include "SplineChainComponent.generated.h"


class USplineComponent;

/**
 * A component that extends UChainComponent to implement a spline-based chain.
 * This component uses a spline to control the movement and weighting of the chain.
 */
UCLASS(HideCategories = (Object, Physics, Activation, "Components|Activation"), EditInlineNew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering, Blueprintable)
class SANDBOXPROJECT_API USplineChainComponent : public UChainComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructor for USplineChainComponent.
	 *
	 * @param ObjectInitializer Used to initialize the object.
	 */
	USplineChainComponent(const FObjectInitializer& ObjectInitializer);

	/**
	 * Called when the component is registered with its owner actor.
	 * This function ensures that the component is properly set up when the game starts or when spawned.
	 */
	virtual void OnRegister() override;
	
	/**
	 * Called every frame. Handles updating the component's behavior.
	 *
	 * @param DeltaTime Time elapsed since last frame.
	 * @param TickType Type of tick for this component.
	 * @param ThisTickFunction Contains tick information for this component.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Initializes the chain, setting up the spline-based logic for the component.
	 */
	virtual void InitChain() override;


	/**
	 * Curve that defines the weight for following the spline. Can be edited in the editor or set at runtime.
	 */	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Chain Component")
	FRuntimeFloatCurve SplineFollowWeight;


private:
	/**
	 * The spline component that defines the path for the chain.
	 */
	UPROPERTY()
	USplineComponent* SplineComponent;

	/**
	 * A float variable used to track or check the spline follow weight internally.
	 */
	float SplineFollowWeightCheck;


};
