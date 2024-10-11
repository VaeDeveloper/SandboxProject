// This is Sandbox Project. 


#include "ThreadComponent.h"
#include "Async/Async.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "UObject/ScriptMacros.h"
#include "Engine/World.h"

// no TAtomic using Epic Games Recomendation using std::atomic 
#include <atomic>

DEFINE_LOG_CATEGORY_STATIC(ThreadLog, All, All);

/**
 * @brief Constructor for UThreadComponent.
 *
 * Initializes the component and enables ticking if necessary.
 */
UThreadComponent::UThreadComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

/**
 * @brief Called when the game starts or when the component is spawned.
 *
 * Initializes thread-based operations and starts asynchronous tasks.
 */
void UThreadComponent::BeginPlay()
{
	Super::BeginPlay();

	//AtomicFunctionTest();

	// Initiates an asynchronous task to draw a point in the world.
	// DrawPointAsyncTask();

	/**
	 * Asynchronous prime calculation task with a callback when the task is completed.
	 * This task runs on a separate thread and logs the result upon completion.
	 */
#pragma region Async
	AsyncThread([&]()->int
		{
			return CalculatePrimes(10000);
		}, 0.0f, TPri_Normal).Then([&](TFuture<int> Future) -> void
			{
				if (Future.IsValid())
				{
					UE_LOG(LogTemp, Warning, TEXT("Calculations Completed"));
					UE_LOG(LogTemp, Warning, TEXT("----------------------"));
					UE_LOG(LogTemp, Warning, TEXT("Result calculated in thread is %i"), Future.Get())

				}
			});

		/**
		 * Another asynchronous task for prime calculation with a callback after task completion.
		 */
		TFuture<int> Result = AsyncThread([&]() ->int
			{
				return CalculatePrimes(20000);
			}, 0.0f, TPri_Normal);

		Result.Next([&](int Number)->void
			{
				UE_LOG(LogTemp, Warning, TEXT("Result calculated in thread is %i"), Number)
			});

		TFuture<int> Result_02 = AsyncThread([&]() -> int
			{
				return CalculatePrimes(10000);
			}, 0.0f, TPri_Normal);

		Result_02.Then([&](TFuture<int> Future)->void
			{
				if (Future.IsValid())
				{
					UE_LOG(LogTemp, Warning, TEXT("Result calculate in thread is %i"), Future.Get())
				}
			});


		/**
		 * A more complex asynchronous task using a thread pool to offload work and then return results.
		 */
		AsyncPool(*GThreadPool, [&]()->int
			{
				return CalculatePrimes(30000);
			}, nullptr, EQueuedWorkPriority::Normal)
			.Next([&](int Nummber)->void
				{
					UE_LOG(LogTemp, Warning, TEXT("Result calculated in thread is %i"), Nummber)
				});


#pragma endregion Async

		// todo 
		// ThreadSafeStackTest();

}

void UThreadComponent::ThreadSafeStackTest()
{
#pragma region ThreadSafe
	/**
	* Sets up a repeating timer task that performs thread-safe operations.
	*/
	GetWorld()->GetTimerManager().SetTimer(*(new FTimerHandle), [&]() ->void
		{
			// Adds items to an array in a thread-safe manner using async tasks.
			Async(EAsyncExecution::TaskGraph, [&]()->void
				{
					ThreadSafeStack.Push(new int(1));
				});
			Async(EAsyncExecution::TaskGraph, [&]()->void
				{
					ThreadSafeStack.Push(new int(2));
				});

			UE_LOG(LogTemp, Error, TEXT("-----------------------"));

			TArray<int*> StackItems;
			ThreadSafeStack.PopAll(StackItems);
			for (int* ArrItem : StackItems)
			{
				UE_LOG(LogTemp, Warning, TEXT("[ArrayItem] = %i"), *ArrItem);
			}

		}, 5.0f, true, 1.0f);
#pragma endregion ThreadSafe
}

void UThreadComponent::AtomicFunctionTest()
{
	std::atomic<int> ThreadCounter{ 0 };

	Async(EAsyncExecution::Thread, [&]()->void
		{
			FGraphEventArray GraphEvents;
			for (int i = 0; i < 200; i++)
			{ // add 200 tasks to graph event list
				GraphEvents.Add(FFunctionGraphTask::CreateAndDispatchWhenReady([&ThreadCounter] {
					FPlatformProcess::Sleep(0.005);
					ThreadCounter++;
					}));
			}
			FTaskGraphInterface::Get().WaitUntilTasksComplete(MoveTemp(GraphEvents), ENamedThreads::AnyThread); // run all tasks in parralel
		});

	Async(EAsyncExecution::Thread, [&]() -> void
		{
			ParallelFor(100, [&ThreadCounter](int32 Index)
				{
					FPlatformProcess::Sleep(0.01f);
					ThreadCounter++;
				});
		});

	FPlatformProcess::Sleep(0.03f);
	UE_LOG(ThreadLog, Warning, TEXT("ThreadCounter Atomic Value is %d"), ThreadCounter.load());
}

void UThreadComponent::ThreadSafeTestFunction()
{

	Async(EAsyncExecution::TaskGraph, [this]()->void {
		FScopeLock ScopeLock{ &Mutex };
		ThreadSafeTst.Add(1);
		});


	Async(EAsyncExecution::TaskGraph, [this]()->void {
		Mutex.Lock();
		ThreadSafeTst.Add(2);
		Mutex.Unlock();
		});

	UE_LOG(LogTemp, Warning, TEXT("-------"))
		// Print Array
		for (int ArrayItem : ThreadSafeTst) 
		{
			FScopeLock ScopeLock{ &Mutex };
			UE_LOG(LogTemp, Warning, TEXT("[ArrayItem] = %i"), ArrayItem)
		}
}

/**
 * @brief Returns a thread-safe copy of the array.
 *
 * This function locks the array using a mutex to ensure thread safety during the copy operation.
 *
 * @return A copy of the thread-safe array.
 */
TArray<int> UThreadComponent::GetThreadSafeArray() const
{
	FScopeLock ScopeLock{ &Mutex };
	return ThreadSafeTst;
}

/**
 * @brief Calculates the first N prime numbers.
 *
 * @param Amount The number of prime numbers to calculate (default is 500).
 * @return The highest prime number found.
 *
 * This function performs CPU-intensive calculations to find prime numbers and logs the time it took to complete.
 */

int UThreadComponent::CalculatePrimes(int Amount)
{
	double StartTime = FPlatformTime::Seconds();
	UE_LOG(ThreadLog, Warning, TEXT("Searching for primes"));

	// Lambda function to check if a number is prime.
	TFunction<bool(int)> IsPrime = [](int num) ->bool
		{
			if (num < 2) return false;

			for (int i = 2; i < num; i++)
			{
				if (num % i == 0) return false;

			}
			return true;
		};

	int PrimesFound = 0;
	int CurrentTestNumber = 0;

	while (PrimesFound < Amount)
	{
		CurrentTestNumber++;
		if (IsPrime(CurrentTestNumber))
		{
			PrimesFound++;
		}
	}
	UE_LOG(ThreadLog, Log, TEXT("Primes Found = %i, Number = %i"), PrimesFound, CurrentTestNumber);

	double EndTime = FPlatformTime::Seconds();
	UE_LOG(ThreadLog, Warning, TEXT("Code Executed in %f seconds. "), EndTime - StartTime);

	return CurrentTestNumber;
}

/**
 * @brief Asynchronously performs background calculations and draws a debug point in the world upon completion.
 *
 * This function demonstrates the use of Unreal Engine's async tasks to perform background work and update the game world afterward.
 */
void UThreadComponent::DrawPointAsyncTask()
{
	const TWeakObjectPtr<UWorld> WeakWorld(GetWorld());

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [WeakWorld]()
		{
			int64 TotalSum = 0;

			for (int32 i = 0; i < 100000000; ++i)
			{
				TotalSum += i;
			}

			UE_LOG(LogTemp, Warning, TEXT("Background calculations completed. Total amount: %lld"), TotalSum);

			AsyncTask(ENamedThreads::GameThread, [WeakWorld, TotalSum]()
				{
					if (WeakWorld.IsValid())
					{
						if (const UWorld* ValidWorld = WeakWorld.Get())
						{
							FVector ResultPoint = FVector(TotalSum % 1000, TotalSum % 1000, 100.0f);
							DrawDebugPoint(ValidWorld, ResultPoint, 50.0f, FColor::Red, false, 10.0f);
							UE_LOG(LogTemp, Warning, TEXT("Result of complex calculations: %lld"), TotalSum);
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("The world was destroyed before the asynchronous task completed.."));
					}
				});
		});

}

/**
 * @brief Asynchronously spawns actors in batches in the game world.
 *
 * @param World Pointer to the current UWorld context.
 * @param ActorClass The class of the actors to be spawned.
 * @param TotalActors The total number of actors to spawn.
 * @param ActorsPerBatch The number of actors to spawn per batch.
 *
 * This function uses async tasks to batch spawn actors in the game world to minimize main thread load.
 */
void UThreadComponent::SpawnActorsAsync(UWorld* World, TSubclassOf<AActor> ActorClass, int32 TotalActors, int32 ActorsPerBatch)
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [World, ActorClass, TotalActors, ActorsPerBatch]()
		{
			int32 Batches = FMath::CeilToInt((float)TotalActors / ActorsPerBatch);
			TArray<FVector> SpawnLocations;

			for (int32 i = 0; i < TotalActors; ++i)
			{
				SpawnLocations.Add(FVector(FMath::RandRange(-1000.0f, 1000.0f), FMath::RandRange(-1000.0f, 1000.0f), 100.0f));
			}

			AsyncTask(ENamedThreads::GameThread, [World, ActorClass, SpawnLocations, ActorsPerBatch]()
				{
					int32 CurrentActorIndex = 0;

					for (int32 i = 0; i < SpawnLocations.Num(); i += ActorsPerBatch)
					{
						for (int32 j = 0; j < ActorsPerBatch && CurrentActorIndex < SpawnLocations.Num(); ++j, ++CurrentActorIndex)
						{
							World->SpawnActor<AActor>(ActorClass, SpawnLocations[CurrentActorIndex], FRotator::ZeroRotator);
						}
						UE_LOG(LogTemp, Warning, TEXT("Spawn Object Count : %d/%d"), CurrentActorIndex, SpawnLocations.Num());
					}
				});
		});
}