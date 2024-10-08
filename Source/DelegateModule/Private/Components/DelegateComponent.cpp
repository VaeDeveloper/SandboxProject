// This is Sandbox Project. 


#include "Components/DelegateComponent.h"

UDelegateComponent::UDelegateComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	DelegateFloatSum.AddDynamic(this, &UDelegateComponent::Summ);

	TUniquePtr<SimpleRawClass> Raw(new SimpleRawClass());
	UsualMulticastSumm.AddRaw(Raw.Get(), &SimpleRawClass::Summ);

	SummStaticTest.AddStatic(&SimpleRawClass::StaticSumm);

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

}

void UDelegateComponent::BeginPlay()
{
	Super::BeginPlay();

	// TestTimerDelegate();
	// ThisComponentGet.Broadcast(this);
	// 
	// UsualSumm delegate execute
	//GetWorld()->GetTimerManager().SetTimer(*(new FTimerHandle()), [this]()->void
	//	{
	//		int UResult;
	//		int a = 3, b = 5;
	//		UsualSumm.ExecuteIfBound(a, b, UResult);
	//		a++, b++;
	//		UE_LOG(LogTemp, Warning, TEXT("Usual summ lambda Result = %d"), UResult);
	//	}, 3.0f, true);
	//
	//// ActorLocationSum delegate execute
	//GetWorld()->GetTimerManager().SetTimer(*(new FTimerHandle()), [this]()->void
	//	{
	//		FVector ResultVector;
	//		const FVector OwnerLocation = GetOwner()->GetActorLocation();
	//		const FVector OwnerRotationVector = GetOwner()->GetActorRotation().Vector();
	//		ActorLocationSum.ExecuteIfBound(OwnerLocation, OwnerRotationVector, ResultVector);
	//		UE_LOG(LogTemp, Warning, TEXT(" Result Vector = %s"), *ResultVector.ToString());
	//	},3.0f, true);
}


void UDelegateComponent::SortActors(UPARAM(ref)TArray<TSoftObjectPtr<AActor>>& ActorList, FDActorsComparator ActorsComparator)
{
	ActorList.Sort([&](TSoftObjectPtr<AActor> A, TSoftObjectPtr<AActor> B)->bool
		{
			bool Result = false;
			ActorsComparator.ExecuteIfBound(A, B, Result);
			return Result;
		});
}

void UDelegateComponent::SummInt(int a, int b, int& c)
{
	c = a + b;
}



void UDelegateComponent::TestTimerDelegate()
{
	GetWorld()->GetTimerManager().SetTimer(*(new FTimerHandle()), [this]()->void
		{
			int UResult;
			int a = 10, b = 20;
			UsualMulticastSumm.Broadcast(a, b, UResult);
			++a, ++b;
			UE_LOG(LogTemp, Warning, TEXT("Result = %d"), UResult);

			if (ArrayAndStringChangeDelegate.IsBound())
			{
				TArray<int> Items = { 5, 4, 2, 1 };
				ArrayAndStringChangeDelegate.Broadcast(Items, TEXT("Test"));

				float Result = 0.0f;
				float A = 2.5f;
				float B = 1.1f;

				DelegateFloatSum.Broadcast(A, B, Result);
				A++; B++;
				UE_LOG(LogTemp, Error, TEXT("Summ Delegate Result = %f"), Result);
			}
		}, 2.5f, true);

		
}

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
