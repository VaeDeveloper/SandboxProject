// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/ScriptMacros.h"
#include "ThreadComponent.generated.h"

/**
 * @class UThreadComponent
 * @brief A custom Unreal Engine component designed to handle threading-related tasks such as asynchronous actor spawning
 * and prime number calculations.
 *
 * This component is part of the "Threads" module and can be spawned in Blueprints. It prioritizes thread management tasks
 * and provides functionality for thread-safe operations within Unreal Engine's Actor system.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, PrioritizeCategories = "Threads", DisplayName = "Thread"))
class THREADSMODULE_API UThreadComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UThreadComponent();

	/**
	 * @brief Asynchronously spawns a specified number of actors in batches.
	 *
	 * @param World Pointer to the current UWorld context.
	 * @param ActorClass The class of the actors to be spawned.
	 * @param TotalActors The total number of actors to spawn.
	 * @param ActorsPerBatch The number of actors to spawn per batch.
	 *
	 * Spawns actors across multiple threads in order to reduce the load on the main thread, improving performance in scenarios
	 * with many actors.
	 */
	void SpawnActorsAsync(UWorld* World, TSubclassOf<AActor> ActorClass, int32 TotalActors, int32 ActorsPerBatch);

	/** 
	 * @brief Thread-safe critical section to protect shared resources.
	 */
	mutable FCriticalSection Mutex;

protected:
	virtual void BeginPlay() override;

	void ThreadSafeStackTest();


	void AtomicFunctionTest();

	void ThreadSafeTestFunction();


	/**
	 * @brief Retrieves a thread-safe copy of an integer array.
	 *
	 * @return A copy of the thread-safe array.
	 *
	 * This function ensures that data access from multiple threads does not cause race conditions.
	 */
	TArray<int> GetThreadSafeArray() const;
	
	/**
	 * @brief Calculates the first N prime numbers.
	 *
	 * @param Amount The number of prime numbers to calculate (default is 500).
	 * @return The total number of primes found.
	 *
	 * This function performs CPU-intensive calculations and can be used to benchmark performance or offload tasks to a background thread.
	 */
	int CalculatePrimes(int Amount = 500);
	
	/**
	 * @brief Asynchronously draws a point using a task-based system.
	 *
	 * This method demonstrates the use of asynchronous tasks in Unreal Engine by drawing points in the game world,
	 * executing the operation in parallel with other game tasks.
	 */
	void DrawPointAsyncTask();

	/**
	 * @brief The total number of primes to calculate in thread-based tasks.
	 *
	 * @note This variable can be set in Blueprints and is used to configure the number of primes for the asynchronous prime calculation task.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Threads|Test")
	int32 Primes = 10000;

	/** 
	 * @brief A thread-safe test TLockFreePointerListLIFO, demonstrating thread-safe access patterns. 
	 */
	TLockFreePointerListLIFO<int> ThreadSafeStack;
	
	/** 
	 *
	 */
	TArray<int> ThreadSafeTst;
	
	/**
	 *
	 */
	int TimerCount = 0;

};
