// This is Sandbox Project. 


#include "ThreadComponent.h"
#include "Async/Async.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "UObject/ScriptMacros.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(ThreadLog, All, All);




UThreadComponent::UThreadComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UThreadComponent::BeginPlay()
{
	Super::BeginPlay();
	// DrawPointAsyncTask();


//#pragma region Async
//	AsyncThread([&]()->int
//		{
//			return CalculatePrimes(10000);
//		}, 0.0f, TPri_Normal).Then([&](TFuture<int> Future) -> void
//			{
//				if (Future.IsValid())
//				{
//					UE_LOG(LogTemp, Warning, TEXT("Calculations Completed"));
//					UE_LOG(LogTemp, Warning, TEXT("----------------------"));
//					UE_LOG(LogTemp, Warning, TEXT("Result calculated in thread is %i"), Future.Get())
//
//				}
//			});
//
//		TFuture<int> Result = AsyncThread([&]() ->int
//			{
//				return CalculatePrimes(20000);
//			}, 0.0f, TPri_Normal);
//
//		Result.Next([&](int Number)->void
//			{
//				UE_LOG(LogTemp, Warning, TEXT("Result calculated in thread is %i"), Number)
//			});
//
//		TFuture<int> Result_02 = AsyncThread([&]() -> int
//			{
//				return CalculatePrimes(10000);
//			}, 0.0f, TPri_Normal);
//
//		Result_02.Then([&](TFuture<int> Future)->void
//			{
//				if (Future.IsValid())
//				{
//					UE_LOG(LogTemp, Warning, TEXT("Result calculate in thread is %i"), Future.Get())
//				}
//			});
//
//		AsyncPool(*GThreadPool, [&]()->int
//			{
//				return CalculatePrimes(30000);
//			}, nullptr, EQueuedWorkPriority::Normal)
//			.Next([&](int Nummber)->void
//				{
//					UE_LOG(LogTemp, Warning, TEXT("Result calculated in thread is %i"), Nummber)
//				});
//
//#pragma endregion Async

	int Count_01 = 0;
	int Count_02 = 0;
	GetWorld()->GetTimerManager().SetTimer(*(new FTimerHandle), [&]() ->void
		{
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

TArray<int> UThreadComponent::GetThreadSafeArray() const
{
	FScopeLock ScopeLock{ &Mutex };
	return ThreadSafeTst;

}

int UThreadComponent::CalculatePrimes(int Amount)
{
	double StartTime = FPlatformTime::Seconds();
	UE_LOG(ThreadLog, Warning, TEXT("Searching for primes"));

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