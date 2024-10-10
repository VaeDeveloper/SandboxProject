// This is Sandbox Project. 


#include "Library/AsyncActionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(AsyncActionLibraryLog, All, All);

// Delay Times for Three Stream 
struct FDelayTimes
{
	float Delay1;
	float Delay2;
	float Delay3;

	FDelayTimes(float InDelay1, float InDelay2)
		: Delay1(InDelay1), Delay2(InDelay1), Delay3(0.0f) {}

	FDelayTimes(float InDelay1, float InDelay2, float InDelay3)
		: Delay1(InDelay1), Delay2(InDelay2), Delay3(InDelay3) {}
};

struct FDelayTimesTwoStreams
{
	float Delay1;
	float Delay2;

	FDelayTimesTwoStreams(const float InDelay1, const float InDelay2)
		: Delay1(InDelay1), Delay2(InDelay2) {}
};


/**
 * @brief A latent action class for managing delayed sequences.
 *
 * This internal class is used by the DelayedSequence function to handle the time-based
 * progression of the delayed action. It monitors elapsed time and triggers specific
 * outputs based on the time that has passed.
 */
void UAsyncActionLibrary::DelayedsequenceThreeStreams(UObject* WorldContextObject, EDelayedInput Inputs, EDelayedExits& Outputs, FLatentActionInfo LatentInfo, float CustomDelay1, float CustomDelay2, float CustomDelay3)
{

	class FDelaySequenceAction : public FPendingLatentAction
	{
		/** The total time that has elapsed since the start of the action. */
		float TotalTime = 0.0f;

		FDelayTimes Delays;

	public:
		/** The name of the execution function to be triggered after the delay. */
		FName ExecutionFunction;

		/** The output link to trigger after the delay completes. */
		int32 OutputLink;

		/** A weak reference to the callback target object. */
		FWeakObjectPtr CallbackTarget;

		/** The delayed input value that determines the delay duration. */
		EDelayedInput Inputs;

		/** A reference to the delayed exit value to output when the sequence completes. */
		EDelayedExits& Outputs;

		/**
		 * @brief Constructor for the delayed sequence action.
		 *
		 * Initializes the latent action with the provided latent info, input value, and output reference.
		 *
		 * @param LatentInfo Information needed for the latent action, including callback and linkage details.
		 * @param InInputs The delay input value.
		 * @param InOutputs A reference to the delayed exit output value.
		 */
		FDelaySequenceAction(const FLatentActionInfo& LatentInfo, EDelayedInput InInputs, EDelayedExits& InOutputs, FDelayTimes& InDelayTimes)
			: ExecutionFunction(LatentInfo.ExecutionFunction)
			, OutputLink(LatentInfo.Linkage)
			, CallbackTarget(LatentInfo.CallbackTarget)
			, Outputs(InOutputs)
			, Inputs(InInputs)
			, Delays(InDelayTimes)
		{
			UE_LOG(AsyncActionLibraryLog, Warning, TEXT("latent action has been started"));
		}

		/** Destructor for the latent action. */
		~FDelaySequenceAction()
		{
			UE_LOG(AsyncActionLibraryLog, Warning, TEXT("latent action has been finished"));
		}

		/**
		 * @brief Updates the latent action operation each tick.
		 *
		 * This method checks the elapsed time since the action started and triggers the corresponding
		 * output based on the selected delay input. It stops the operation once the correct time is reached.
		 *
		 * @param Response The latent response handler used to control the flow of the action.
		 */
		void UpdateOperation(FLatentResponse& Response) override
		{
			TotalTime += Response.ElapsedTime();

			// —писок задержек и проверка дл€ выполнени€ условий
			const TArray<float> DelayTimes = { Delays.Delay1, Delays.Delay2, Delays.Delay3 };

			const TArray<EDelayedInput> InputConditions = { EDelayedInput::Execute, EDelayedInput::Delay1, EDelayedInput::Delay2 };
			const TArray<EDelayedExits> OutputConditions = { EDelayedExits::Then, EDelayedExits::Exit1, EDelayedExits::Exit2 };


			for (int32 i = 0; i < DelayTimes.Num(); ++i)
			{
				if (TotalTime >= DelayTimes[i] && Inputs == InputConditions[i])
				{
					Outputs = OutputConditions[i];
					Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
					return;
				}
			}

			float StopFlowCooldown = 1.0f;
			Response.DoneIf(TotalTime > Delays.Delay3 + StopFlowCooldown);
		}

		/** Notifies that the object has been destroyed, cleaning up the latent action. */
		virtual void NotifyObjectDestroyed() override
		{
			//UE_LOG(AsyncActionLibraryLog, Warning, TEXT("Latent action already exists for UUID: %d"), LatentInfo.UUID);
		}

		/** Notifies that the action has been aborted, cleaning up the latent action. */
		virtual void NotifyActionAborted() override
		{
			// UE_LOG(AsyncActionLibraryLog, Warning, TEXT("Latent action already exists for UUID: %d"), LatentInfo.UUID);
		}
	};

	// Check if the world context object is valid and retrieve the latent action manager
	if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FDelaySequenceAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			FDelayTimes Delays(CustomDelay1, CustomDelay2, CustomDelay3);

			TUniquePtr<FDelaySequenceAction> NewAction = MakeUnique<FDelaySequenceAction>(LatentInfo, Inputs, Outputs, Delays);
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

void UAsyncActionLibrary::DelayedSequenceTwoStreams(UObject* WorldContextObject, EDelayedInputTwoStream Inputs, EDelayedExitsTwoStream& Outputs, FLatentActionInfo LatentInfo, float CustomDelay1, float CustomDelay2)
{
	class FDelayActionSequence : public FPendingLatentAction
	{
		FDelayTimesTwoStreams DelayTimes2Sreams;

		float TotalTimes = 0.0f;

	public:

		/** The name of the execution function to be triggered after the delay. */
		FName ExecutionFunction;

		/** The output link to trigger after the delay completes. */
		int32 OutputLink;

		/** A weak reference to the callback target object. */
		FWeakObjectPtr CallbackTarget;

		/** The delayed input value that determines the delay duration. */
		EDelayedInputTwoStream Inputs;

		/** A reference to the delayed exit value to output when the sequence completes. */
		EDelayedExitsTwoStream& Outputs;

		FDelayActionSequence(const FLatentActionInfo& LatentInfo, EDelayedInputTwoStream InInputs, EDelayedExitsTwoStream& InOutputs, FDelayTimesTwoStreams& InDelayTimes)
			: ExecutionFunction(LatentInfo.ExecutionFunction)
			, OutputLink(LatentInfo.Linkage)
			, CallbackTarget(LatentInfo.CallbackTarget)
			, Outputs(InOutputs)
			, Inputs(InInputs)
			, DelayTimes2Sreams(InDelayTimes)
		{
			UE_LOG(AsyncActionLibraryLog, Log, TEXT("latent action has been started"));
		}

		~FDelayActionSequence()
		{
			UE_LOG(AsyncActionLibraryLog, Log, TEXT("FDelayActionSequence Destructor call "));
		}

		void UpdateOperation(FLatentResponse& Response)
		{
			TotalTimes += Response.ElapsedTime();

			// —писок задержек и проверка дл€ выполнени€ условий
			const TArray<float> DelayTimes = { DelayTimes2Sreams.Delay1, DelayTimes2Sreams.Delay2};

			const TArray<EDelayedInputTwoStream> InputConditions = { EDelayedInputTwoStream::Execute, EDelayedInputTwoStream::Delay1 };
			const TArray<EDelayedExitsTwoStream> OutputConditions = { EDelayedExitsTwoStream::Then, EDelayedExitsTwoStream::Exit1};


			for (int32 i = 0; i < DelayTimes.Num(); ++i)
			{
				if (TotalTimes >= DelayTimes[i] && Inputs == InputConditions[i])
				{
					Outputs = OutputConditions[i];
					Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
					return;
				}
			}

			float StopFlowCooldown = 1.0f;
			Response.DoneIf(TotalTimes > DelayTimes2Sreams.Delay2 + StopFlowCooldown);
		}

		virtual void NotifyObjectDestroyed() override
		{
			UE_LOG(AsyncActionLibraryLog, Log, TEXT("Object Destroyed called "));
		}

		virtual void NotifyActionAborted() override
		{
			UE_LOG(AsyncActionLibraryLog, Log, TEXT("Object Aborted called "));
		}
	};
}
