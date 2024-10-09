// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/LatentActionManager.h"
#include "LatentActions.h"

#include "AsyncActionLibrary.generated.h"

/**
 * @brief Enumeration for delayed input actions.
 *
 * This enum defines different delayed input types that can be used in the delayed sequence.
 * Each enum value represents a specific delay duration that the system waits before executing.
 */
UENUM(BlueprintType)
enum class EDelayedInput : uint8
{
	/** Executes after a delay of 0.5 seconds. */
	Execute UMETA(DisplayName = "Delay 0.5"),
	
	/** Executes after a delay of 1 second. */
	Delay1  UMETA(DisplayName = "Delay 1"),
	
	/** Executes after a delay of 2 seconds. */
	Delay2 UMETA(DisplayName = "Delay 2")
};

/**
 * @brief Enumeration for delayed exit actions.
 *
 * This enum defines the possible exit points for a delayed action sequence.
 * Each value represents a specific exit state based on the time elapsed.
 */
UENUM(BlueprintType)
enum class EDelayedExits : uint8
{
	/** Exits after 0.5 seconds. */
	Then UMETA(DisplayName = "Exit 0.5"),
	
	/** Exits after 1 second. */
	Exit1 UMETA(DisplayName = "Exit 1"),

	/** Exits after 2 seconds. */
	Exit2 UMETA(DisplayName = "Exit 2")
};



/**
 * @brief A blueprint function library for handling asynchronous actions with delays.
 *
 * This class provides utilities for handling latent and delayed action sequences
 * that can be used in Unreal Engine's blueprint system. It allows for executing
 * functions with various delays.
 */
UCLASS()
class ASYNCACTIONS_API UAsyncActionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	
	/**
	 * @brief Initiates a delayed sequence with input and output handling.
	 *
	 * This static function allows blueprints to trigger a delayed action sequence.
	 * Based on the selected input delay, it will execute the sequence after the
	 * specified time and output the corresponding delayed exit.
	 *
	 * @param WorldContextObject The world context object from which this latent action is called.
	 * @param Inputs The delay input value that specifies how long the sequence should wait before proceeding.
	 * @param Outputs The output exit value that indicates the result of the delayed sequence.
	 * @param LatentInfo Information required for managing latent action flow, including execution and linkage data.
	 */
	UFUNCTION(BlueprintCallable, Category = "LatentActions|DelaySequence", meta = (ExpandEnumAsExecs = "Inputs, Outputs", Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject", KeyWords = "sleep"))
	static void DelayedSequence(UObject* WorldContextObject, EDelayedInput Inputs, EDelayedExits& Outputs, FLatentActionInfo LatentInfo);
	
};
