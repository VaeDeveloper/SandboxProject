// This is Sandbox Project. 


#include "Library/AsyncActionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(AsyncActionLibraryLog, All, All);

/**
 * @brief A latent action class for managing delayed sequences.
 *
 * This internal class is used by the DelayedSequence function to handle the time-based
 * progression of the delayed action. It monitors elapsed time and triggers specific
 * outputs based on the time that has passed.
 */
void UAsyncActionLibrary::DelayedSequence(UObject* WorldContextObject, EDelayedInput Inputs, EDelayedExits& Outputs, FLatentActionInfo LatentInfo)
{
	class FDelaySequenceAction : public FPendingLatentAction
	{
		/** The total time that has elapsed since the start of the action. */
		float TotalTime = 0.0f;

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

		/** A flag indicating if the half-second trigger has occurred. */
		bool bHalfTriggered = false;
		
		/**
		 * @brief Constructor for the delayed sequence action.
		 *
		 * Initializes the latent action with the provided latent info, input value, and output reference.
		 *
		 * @param LatentInfo Information needed for the latent action, including callback and linkage details.
		 * @param InInputs The delay input value.
		 * @param InOutputs A reference to the delayed exit output value.
		 */
		FDelaySequenceAction(const FLatentActionInfo& LatentInfo, EDelayedInput InInputs, EDelayedExits& InOutputs)
			: ExecutionFunction(LatentInfo.ExecutionFunction)
			, OutputLink(LatentInfo.Linkage)
			, CallbackTarget(LatentInfo.CallbackTarget)
			, Outputs(InOutputs)
			, Inputs(InInputs)
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
			
			bool condition2 = TotalTime >= 2.0f && Inputs == EDelayedInput::Delay2;
			bool condition1 = TotalTime >= 1.0f && Inputs == EDelayedInput::Delay1;
			bool condition05 = TotalTime >= 0.5f && Inputs == EDelayedInput::Execute;

			if (condition2)
			{
				Outputs = EDelayedExits::Exit2;
				Response.FinishAndTriggerIf(condition2, ExecutionFunction, OutputLink, CallbackTarget);
			}
			else if (condition1)
			{
				Outputs = EDelayedExits::Exit1;
				Response.FinishAndTriggerIf(condition1, ExecutionFunction, OutputLink, CallbackTarget);
			}
			else if (condition05)
			{
				Outputs = EDelayedExits::Then;
				Response.FinishAndTriggerIf(condition05, ExecutionFunction, OutputLink, CallbackTarget);
			}
			else
			{
				Response.DoneIf(TotalTime > 3);
			}
		}

		/** Notifies that the object has been destroyed, cleaning up the latent action. */
		virtual void NotifyObjectDestroyed() override
		{
			delete this;
		}

		/** Notifies that the action has been aborted, cleaning up the latent action. */
		virtual void NotifyActionAborted() override
		{
			delete this;
		}
	};

	// Check if the world context object is valid and retrieve the latent action manager
	if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager(); //Get LatentActionManager
		if (LatentActionManager.FindExistingAction<FDelaySequenceAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == NULL)
		{
			// Add the new FDelaySequenceAction to the LatentActionManager
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FDelaySequenceAction(LatentInfo, Inputs, Outputs));
		}
	}
}
