// This is Sandbox Project. 


#include "Library/AsyncActionLibrary.h"

DEFINE_LOG_CATEGORY(AsyncActionLibraryLog);

/**
 * @brief A latent action class for managing delayed sequences.
 *
 * This internal class is used by the DelayedSequence function to handle the time-based
 * progression of the delayed action. It monitors elapsed time and triggers specific
 * outputs based on the time that has passed.
 */
void UAsyncActionLibrary::DelayedSequenceThreeStreams(UObject* WorldContextObject, EDelayedInputThreeStream Inputs, EDelayedExitsThreeStream& Outputs, FLatentActionInfo LatentInfo, float CustomDelay1, float CustomDelay2, float CustomDelay3)
{
	TArray<float> Delays = { CustomDelay1, CustomDelay2, CustomDelay3 };
	TArray<EDelayedInputThreeStream> InputConditions = { EDelayedInputThreeStream::Execute, EDelayedInputThreeStream::Delay1, EDelayedInputThreeStream::Delay2 };
	TArray<EDelayedExitsThreeStream> OutputConditions = { EDelayedExitsThreeStream::Then, EDelayedExitsThreeStream::Exit1, EDelayedExitsThreeStream::Exit2 };

	GeneralizedDelayedSequence<EDelayedInputThreeStream, EDelayedExitsThreeStream>(WorldContextObject, LatentInfo, Delays, InputConditions, Outputs, OutputConditions);
}


void UAsyncActionLibrary::DelayedSequenceTwoStreams(UObject* WorldContextObject, EDelayedInputTwoStream Inputs, EDelayedExitsTwoStream& Outputs, FLatentActionInfo LatentInfo, float CustomDelay1, float CustomDelay2)
{
	TArray<float> Delays = { CustomDelay1, CustomDelay2 };
	TArray<EDelayedInputTwoStream> InputConditions = { EDelayedInputTwoStream::Execute, EDelayedInputTwoStream::Delay1 };
	TArray<EDelayedExitsTwoStream> OutputConditions = { EDelayedExitsTwoStream::Then, EDelayedExitsTwoStream::Exit1 };

	GeneralizedDelayedSequence<EDelayedInputTwoStream, EDelayedExitsTwoStream>(WorldContextObject, LatentInfo, Delays, InputConditions, Outputs, OutputConditions);
}
