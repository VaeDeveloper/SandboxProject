// This is Sandbox Project.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/LatentActionManager.h"
#include "LatentActions.h"

#include "AsyncActionLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AsyncActionLibraryLog, Log, All);

class FDelaySequenceAction;


UENUM(BlueprintType)
enum class EDelayedInputTwoStream : uint8
{
	/** Executes after a delay of 0.5 seconds. */
	Execute UMETA(DisplayName = "Delay 0.5"),

	/** Executes after a delay of 1 second. */
	Delay1 UMETA(DisplayName = "Delay 1"),
};

UENUM(BlueprintType)
enum class EDelayedExitsTwoStream : uint8
{
	/** Exits after 0.5 seconds. */
	Then UMETA(DisplayName = "Exit 0.5"),

	/** Exits after 1 second. */
	Exit1 UMETA(DisplayName = "Exit 1"),
};

/**
 * @brief Enumeration for delayed input actions.
 *
 * This enum defines different delayed input types that can be used in the delayed sequence.
 * Each enum value represents a specific delay duration that the system waits before executing.
 */
UENUM(BlueprintType)
enum class EDelayedInputThreeStream : uint8
{
	/** Executes after a delay of 0.5 seconds. */
	Execute UMETA(DisplayName = "Delay 0.5"),

	/** Executes after a delay of 1 second. */
	Delay1 UMETA(DisplayName = "Delay 1"),

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
enum class EDelayedExitsThreeStream : uint8
{
	/** Exits after 0.5 seconds. */
	Then UMETA(DisplayName = "Exit 0.5"),

	/** Exits after 1 second. */
	Exit1 UMETA(DisplayName = "Exit 1"),

	/** Exits after 2 seconds. */
	Exit2 UMETA(DisplayName = "Exit 2")
};

// Delay Times for Three Stream
struct FDelayTimesTthreeStream
{
	float Delay1;
	float Delay2;
	float Delay3;

	FDelayTimesTthreeStream(float InDelay1, float InDelay2) : Delay1(InDelay1), Delay2(InDelay1), Delay3(0.0f) {}

	FDelayTimesTthreeStream(float InDelay1, float InDelay2, float InDelay3) : Delay1(InDelay1), Delay2(InDelay2), Delay3(InDelay3) {}
};

struct FDelayTimesTwoStreams
{
	float Delay1;
	float Delay2;

	FDelayTimesTwoStreams(const float InDelay1, const float InDelay2) : Delay1(InDelay1), Delay2(InDelay2) {}
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
	 * @brief Initiates a delayed sequence with three input and output streams.
	 *
	 * This function triggers a delayed action sequence with three different delay streams.
	 * It waits for the specified input delay before proceeding to the output stage.
	 *
	 * @param WorldContextObject The world context object.
	 * @param Inputs The input delay that determines when to trigger the output.
	 * @param Outputs The output delay based on the time elapsed.
	 * @param LatentInfo Latent action info for Unreal's latent action system.
	 * @param CustomDelay1 Custom delay for the first stream (default is 0.5 seconds).
	 * @param CustomDelay2 Custom delay for the second stream (default is 1 second).
	 * @param CustomDelay3 Custom delay for the third stream (default is 2 seconds).
	 */
	UFUNCTION(BlueprintCallable, Category = "LatentActions|DelaySequence", meta = (ExpandEnumAsExecs = "Inputs, Outputs", Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject", KeyWords = "sleep"))
	static void DelayedSequenceThreeStreams(UObject* WorldContextObject, EDelayedInputThreeStream Inputs, EDelayedExitsThreeStream& Outputs, FLatentActionInfo LatentInfo, float CustomDelay1 = 0.5f, float CustomDelay2 = 1.0f, float CustomDelay3 = 2.0f);

	/**
	 * @brief Initiates a delayed sequence with two input and output streams.
	 *
	 * This function triggers a delayed action sequence with two different delay streams.
	 * It waits for the specified input delay before proceeding to the output stage.
	 *
	 * @param WorldContextObject The world context object.
	 * @param Inputs The input delay that determines when to trigger the output.
	 * @param Outputs The output delay based on the time elapsed.
	 * @param LatentInfo Latent action info for Unreal's latent action system.
	 * @param CustomDelay1 Custom delay for the first stream (default is 0.5 seconds).
	 * @param CustomDelay2 Custom delay for the second stream (default is 1 second).
	 */
	UFUNCTION(BlueprintCallable, Category = "LatentActions|DelaySequence", meta = (ExpandEnumAsExecs = "Inputs, Outputs", Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject", KeyWords = "sleep"))
	static void DelayedSequenceTwoStreams(UObject* WorldContextObject, EDelayedInputTwoStream Inputs, EDelayedExitsTwoStream& Outputs, FLatentActionInfo LatentInfo, float CustomDelay1 = 0.5f, float CustomDelay2 = 1.0f);

private:
	/**
	 * @brief Generalized delayed sequence handler for arbitrary streams.
	 *
	 * This function handles delayed sequences of arbitrary lengths and simplifies
	 * the logic for both two- and three-stream delayed sequences by using generic
	 * arrays for inputs and outputs.
	 *
	 * @param WorldContextObject The world context object.
	 * @param LatentInfo Latent action info for Unreal's latent action system.
	 * @param DelayTimes Array of custom delay times.
	 * @param InputsConditions Array of input delay enums for the corresponding streams.
	 * @param OutputsConditions Array of output enums to match the input conditions.
	 */
	template <typename InputEnum, typename OutputEnum>
	static void GeneralizedDelayedSequence(UObject* WorldContextObject, FLatentActionInfo LatentInfo, const TArray<float>& DelayTimes, const TArray<InputEnum>& InputsConditions, OutputEnum& Outputs, const TArray<OutputEnum>& OutputsConditions);
};
///////////////////////////////////////////////////////////////////////////////////////////////////
// TODO !!! надо доделать, ноды не работают так как до рефакторинга... Ошибка где то в шаблоне ....
template <typename InputEnum, typename OutputEnum>
inline void UAsyncActionLibrary::GeneralizedDelayedSequence(UObject* WorldContextObject, FLatentActionInfo LatentInfo, const TArray<float>& DelayTimes, const TArray<InputEnum>& InputsConditions, OutputEnum& Outputs, const TArray<OutputEnum>& OutputsConditions)
{
	if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FDelaySequenceAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			class FDelaySequenceAction : public FPendingLatentAction
			{
				/** Total time elapsed */
				float TotalTime = 0.0f;

				/** Delay times */
				TArray<float> Delays;

				/** Input and output conditions */
				TArray<InputEnum> InputsConditions;
				OutputEnum& Outputs;
				TArray<OutputEnum> OutputsConditions;

				FName ExecutionFunction;
				int32 OutputLink;
				FWeakObjectPtr CallbackTarget;

			public:
				FDelaySequenceAction(const FLatentActionInfo& LatentInfo, OutputEnum& InOutputs, const TArray<float>& InDelays, const TArray<InputEnum>& InInputsConditions, const TArray<OutputEnum>& InOutputsConditions)
					: Outputs(InOutputs), Delays(InDelays), InputsConditions(InInputsConditions), OutputsConditions(InOutputsConditions), ExecutionFunction(LatentInfo.ExecutionFunction), OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget)
				{
					// constructor logic
				}

				~FDelaySequenceAction() = default;

				void UpdateOperation(FLatentResponse& Response) override
				{
					TotalTime += Response.ElapsedTime();

					for (int32 i = 0; i < Delays.Num(); ++i)
					{
						if (TotalTime >= Delays[i] && InputsConditions[i] == InputsConditions[i])
						{
							Outputs = OutputsConditions[i];
							Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
							return;
						}
					}

					Response.DoneIf(TotalTime > Delays.Last() + 1.0f);	// Stop cooldown
				}
			};

			TUniquePtr<FDelaySequenceAction> NewAction = MakeUnique<FDelaySequenceAction>(LatentInfo, Outputs, DelayTimes, InputsConditions, OutputsConditions);
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, NewAction.Release());
		}
		else
		{
			UE_LOG(AsyncActionLibraryLog, Warning, TEXT("Latent action already exists for UUID: %d"), LatentInfo.UUID);
		}
	}
	else
	{
		UE_LOG(AsyncActionLibraryLog, Error, TEXT("Failed to get world context object"));
	}
}
