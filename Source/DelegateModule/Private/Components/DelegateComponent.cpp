// This is Sandbox Project. 


#include "Components/DelegateComponent.h"

namespace
{
	constexpr float UsualSummRate			= 3.0f;
	constexpr float ActorLocationSumRate	= 3.0f;
	constexpr float TestTimerDelegateRate	= 2.5f;
}

/**
 * @brief Constructor for UDelegateComponent.
 *
 * Initializes the component and binds various delegates.
 * Sets up timer-based operations and delegates for summing and manipulating vectors.
 *
 * @param ObjectInitializer Initialization parameters for the component.
 */
UDelegateComponent::UDelegateComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	DelegateFloatSum.AddDynamic(this, &UDelegateComponent::Summ);

	TUniquePtr<SimpleRawClass> Raw(new SimpleRawClass());
	UsualMulticastSumm.AddRaw(Raw.Get(), &SimpleRawClass::Summ);

	SummStaticTest.AddStatic(&SimpleRawClass::StaticSumm);


	/* clang-format off */
	UsualSumm.BindLambda([&](int a, int b, int& c)->void 
		{
			c = a + b;
		});

	ActorLocationSum.BindLambda([&](FVector A, FVector B, FVector& Result)
		{
			Result = A + B;
		});

	SimpleDel.BindLambda([]()->void
		{
			UE_LOG(LogTemp, Warning, TEXT("Simpe Delegate Called"));
		});
	/* clang-format on */

}

/**
 * @brief Called when the game starts or when the component is spawned.
 *
 * Initializes timer-based operations that execute bound delegates for summing and logging.
 */
void UDelegateComponent::BeginPlay()
{
	Super::BeginPlay();

	TestTimerDelegate();
	ThisComponentGet.Broadcast(this);
	 
	// UsualSumm delegate execute
	GetWorld()->GetTimerManager().SetTimer(*(new FTimerHandle()), [this]()->void
		{
			int UResult;
			int a = 3, b = 5;

			UsualSumm.ExecuteIfBound(a, b, UResult);

			UE_LOG(LogTemp, Warning, TEXT("Usual summ lambda Result = %d"), UResult);
		}, UsualSummRate, true);
	
	// ActorLocationSum delegate execute
	GetWorld()->GetTimerManager().SetTimer(*(new FTimerHandle()), [this]()->void
		{
			FVector ResultVector;
			const FVector OwnerLocation = GetOwner()->GetActorLocation();
			const FVector OwnerRotationVector = GetOwner()->GetActorRotation().Vector();

			ActorLocationSum.ExecuteIfBound(OwnerLocation, OwnerRotationVector, ResultVector);

			UE_LOG(LogTemp, Warning, TEXT(" Result Vector = %s"), *ResultVector.ToString());
		}, UsualSummRate, true);
}

/**
 * @brief Sorts a list of actors using a bound delegate for comparison.
 *
 * @param ActorList A reference to an array of soft pointers to actors to sort.
 * @param ActorsComparator A delegate that defines the comparison logic.
 */
void UDelegateComponent::SortActors(UPARAM(ref)TArray<TSoftObjectPtr<AActor>>& ActorList, FDActorsComparator ActorsComparator)
{
	ActorList.Sort([&](TSoftObjectPtr<AActor> A, TSoftObjectPtr<AActor> B)->bool
		{
			bool Result = false;
			ActorsComparator.ExecuteIfBound(A, B, Result);
			return Result;
		});
}

/**
 * @brief Sums two integers and stores the result in a reference parameter.
 *
 * @param a The first integer.
 * @param b The second integer.
 * @param c The result of the sum, passed by reference.
 */
void UDelegateComponent::SummInt(int a, int b, int& c)
{
	c = a + b;
}

/**
 * @brief Initializes a repeating timer that executes a test for the timer delegate.
 *
 * This function broadcasts multicast delegates, executes usual summing,
 * and logs the results at defined intervals.
 */
void UDelegateComponent::TestTimerDelegate()
{
	GetWorld()->GetTimerManager().SetTimer(*(new FTimerHandle()), [this]()->void
		{
			int UResult;
			int a = 10, b = 20;
			
			UsualMulticastSumm.Broadcast(a, b, UResult);
			

			UE_LOG(LogTemp, Warning, TEXT("Result = %d"), UResult);

			if (ArrayAndStringChangeDelegate.IsBound())
			{
				const TArray<int> Items = { 5, 4, 2, 1 };
				ArrayAndStringChangeDelegate.Broadcast(Items, TEXT("Test"));

				float Result = 0.0f;
				float A = 2.5f;
				float B = 1.1f;

				DelegateFloatSum.Broadcast(A, B, Result);
				A++; B++;

				UE_LOG(LogTemp, Error, TEXT("Summ Delegate Result = %f"), Result);
			}
		}, TestTimerDelegateRate, true);
}

/**
 * @brief Executes a threaded delay and binds the result to a delegate upon completion.
 *
 * @param DelayDuration The duration to wait before completing the task.
 * @param ResultDelegate The delegate to execute with the result after the delay.
 */
void UDelegateComponent::MakeThreadedDelay(float DelayDuration, FCallResultDelegate ResultDelegate)
{
	FSimpleDelegate Delegate;
	Async(EAsyncExecution::Thread, [&]()->FString
		{
			FPlatformProcess::Sleep(DelayDuration);
			return "Thread Finished ";
		}).Then([Delegate = MoveTemp(ResultDelegate)](TFuture<FString> Result) -> void
			{
				if (Result.IsValid())
				{
					Delegate.ExecuteIfBound(Result.Get());
				}
			});

}
