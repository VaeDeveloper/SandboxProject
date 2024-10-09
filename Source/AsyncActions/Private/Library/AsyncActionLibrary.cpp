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
void UAsyncActionLibrary::DelayedSequence(UObject* WorldContextObject, EDelayedInput Inputs, EDelayedExits& Outputs, FLatentActionInfo LatentInfo, float CustomDelay1 , float CustomDelay2, float CustomDelay3)
{

	struct FDelayTimes
	{
		float Delay1;
		float Delay2;
		float Delay3;

		FDelayTimes(float InDelay1, float InDelay2, float InDelay3)
			: Delay1(InDelay1), Delay2(InDelay2), Delay3(InDelay3) {}
	};

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
			
			// ������ �������� � �������� ��� ���������� �������
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

			Response.DoneIf(TotalTime > Delays.Delay3 + 1.0f);
		}

		/** Notifies that the object has been destroyed, cleaning up the latent action. */
		virtual void NotifyObjectDestroyed() override
		{
			
		}

		/** Notifies that the action has been aborted, cleaning up the latent action. */
		virtual void NotifyActionAborted() override
		{
		}
	};

	// Check if the world context object is valid and retrieve the latent action manager
	if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FDelaySequenceAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			// �������� ��������� ��������
			FDelayTimes Delays(CustomDelay1, CustomDelay2, CustomDelay3);

			// �������� � ���������� ����� �������� � LatentActionManager
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
	}}
