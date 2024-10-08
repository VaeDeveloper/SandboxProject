// This is Sandbox Project. 


#include "ThreadComponent.h"
#include "Async/Async.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "UObject/ScriptMacros.h"
#include "Engine/World.h"

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

	// Initiates an asynchronous task to draw a point in the world.
	DrawPointAsyncTask();

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

	/**
	 * Sets up a repeating timer task that performs thread-safe operations.
	 */
	int Count_01 = 0;
	int Count_02 = 0;
	GetWorld()->GetTimerManager().SetTimer(*(new FTimerHandle), [&]() ->void
		{
			// Adds items to an array in a thread-safe manner using async tasks.
			Async(EAsyncExecution::TaskGraph, [&]()->void
				{
					FScopeLock ScopeLock{ &Mutex };
					ThreadSafeTst.Add(Count_01);
					Count_01++;

				});
			Async(EAsyncExecution::TaskGraph, [&]()->void
				{
					Mutex.Lock();
					ThreadSafeTst.Add(Count_02);
					Count_02++;
					Mutex.Unlock();
				});        

			UE_LOG(LogTemp, Error, TEXT("-----------------------"));

			for (int ArrItem : ThreadSafeTst)
			{
				FScopeLock ScopeLock{ &Mutex };
				UE_LOG(LogTemp, Warning, TEXT("[ArrayItem] = %i"), ArrItem);
			}

		}, 5.0f, true, 1.0f);

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