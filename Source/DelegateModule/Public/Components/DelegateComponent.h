// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "DelegateComponent.generated.h"


USTRUCT(BlueprintType)
struct FMapContainer
{
	GENERATED_BODY()

	FMapContainer() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int> DelegateNameIntMap;
};

class SimpleRawClass
{
public:
	void Summ(int a, int b, int& c)
	{
		c = a + b;
	}

	static void StaticSumm(int a, int b, int& c)
	{
		c = a + b;
	}
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FCallResultDelegate, FString, FinishMessage);
DECLARE_MULTICAST_DELEGATE_ThreeParams(DFUsualMulticastSumm, int NumA, int NumB, int& Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateComponentGet, const UActorComponent*, DelegateActorComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FArrayAndStrChange, const TArray<int>&, IntArrayData, FString, StringData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDFloatSumm, float, a, float, b, float&, c);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDMapTest, const TArray<int>&, ArrayData, const FString&, StringData, FMapContainer, MapData);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDActorsComparator, TSoftObjectPtr<AActor>, FirstActor, TSoftObjectPtr<AActor>, SecondActor, bool&, Result);
DECLARE_DELEGATE_ThreeParams(DFUsualSumm, int NumA, int NumB, int& Result);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FSummStaticTest, int NumA, int NumB, int& NumC);

DECLARE_DELEGATE_ThreeParams(FActorLocationSum, FVector VectorA, FVector VectorB, FVector& Result);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, PrioritizeCategories = "DelegateComponent"))
class DELEGATEMODULE_API UDelegateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDelegateComponent(const FObjectInitializer& ObjectInitializer);


	FSimpleDelegate SimpleDel;

	UPROPERTY(BlueprintAssignable, Category = "DelegateComponent|Delegates")
	FArrayAndStrChange ArrayAndStringChangeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "DelegateComponent|Delegates")
	FDelegateComponentGet ThisComponentGet;
	
	UPROPERTY(BlueprintAssignable, Category = "DelegateComponent|Delegates")
	FDFloatSumm DelegateFloatSum;

	UFUNCTION(BlueprintCallable, Category = "DelegateComponent|Sort")
	void SortActors(UPARAM(ref) TArray<TSoftObjectPtr<AActor>>& ActorList, FDActorsComparator ActorsComparator);

	DFUsualMulticastSumm UsualMulticastSumm;
	DFUsualSumm UsualSumm;
	FActorLocationSum ActorLocationSum;
	FSummStaticTest SummStaticTest;

	UFUNCTION(BlueprintCallable, Category = "SummInt")
	void SummInt(int a, int b, int& c);

	UFUNCTION() 
	void Summ(float a, float b, float& c)
	{
		c = a + b;
		UE_LOG(LogTemp, Error, TEXT("Summ is = %f"), c);
	}

protected:
	virtual void BeginPlay() override;
private:
	void TestTimerDelegate();

	

	static void MakeThreadedDelay(float DelayDuration, FCallResultDelegate ResultDelegate);

};
