// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/ScriptMacros.h"
#include "ThreadComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, PrioritizeCategories = "Threads", DisplayName = "Thread"))
class THREADSMODULE_API UThreadComponent : public UActorComponent
{
	GENERATED_BODY()

public:



public:	
	UThreadComponent();

	void SpawnActorsAsync(UWorld* World, TSubclassOf<AActor> ActorClass, int32 TotalActors, int32 ActorsPerBatch);
	
	mutable FCriticalSection Mutex;
protected:
	virtual void BeginPlay() override;

	TArray<int> GetThreadSafeArray() const;


	int CalculatePrimes(int Amount = 500);
	void DrawPointAsyncTask();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Threads|Test")
	int32 Primes = 10000;

	TArray<int> ThreadSafeTst;

};
