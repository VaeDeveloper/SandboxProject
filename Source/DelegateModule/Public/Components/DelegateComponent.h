// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "DelegateComponent.generated.h"

/**
 * @brief A structure that holds a map of delegate names to integer values.
 *
 * This structure can be used to store pairs of names and corresponding integer values,
 * enabling dynamic retrieval of integers associated with specific delegate names.
 */
USTRUCT(BlueprintType)
struct FMapContainer
{
	GENERATED_BODY()

	FMapContainer() {};

	/**
	 * @brief A map that associates FName keys with integer values.
	 *
	 * This map is editable in the editor and can be read/written in Blueprints.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int> DelegateNameIntMap;
};


/**
 * @brief A simple class that provides summation methods.
 *
 * This class includes both instance and static methods for summing two integers.
 */
class SimpleRawClass
{
public:
	/**
	 * @brief Sums two integers and stores the result in a reference parameter.
	 *
	 * @param a The first integer.
	 * @param b The second integer.
	 * @param c The result of the sum, passed by reference.
	 */
	void Summ(int a, int b, int& c)
	{
		c = a + b;
	}
	
	/**
	 * @brief A static method that sums two integers and stores the result in a reference parameter.
	 *
	 * @param a The first integer.
	 * @param b The second integer.
	 * @param c The result of the sum, passed by reference.
	 */
	static void StaticSumm(int a, int b, int& c)
	{
		c = a + b;
	}
};


/**
 * @brief A delegate that is called with a single FString parameter.
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FCallResultDelegate, FString, FinishMessage);

/**
 * @brief A multicast delegate that takes three integer parameters and modifies a result.
 */
DECLARE_MULTICAST_DELEGATE_ThreeParams(DFUsualMulticastSumm, int NumA, int NumB, int& Result);

/**
 * @brief A multicast delegate that broadcasts a pointer to an UActorComponent.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateComponentGet, const UActorComponent*, DelegateActorComponent);

/**
 * @brief A multicast delegate that takes an integer array and a string.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FArrayAndStrChange, const TArray<int>&, IntArrayData, FString, StringData);

/**
 * @brief A multicast delegate that takes two float parameters and modifies a result.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDFloatSumm, float, a, float, b, float&, c);

/**
 * @brief A multicast delegate that takes an integer array, a string, and an FMapContainer.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDMapTest, const TArray<int>&, ArrayData, const FString&, StringData, FMapContainer, MapData);

/**
 * @brief A delegate that compares two actors and modifies a boolean result.
 */
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDActorsComparator, TSoftObjectPtr<AActor>, FirstActor, TSoftObjectPtr<AActor>, SecondActor, bool&, Result);

/**
 * @brief A delegate that sums two integers and modifies a result.
 */
DECLARE_DELEGATE_ThreeParams(DFUsualSumm, int NumA, int NumB, int& Result);

/**
 * @brief A multicast delegate for a static summation test.
 */
DECLARE_MULTICAST_DELEGATE_ThreeParams(FSummStaticTest, int NumA, int NumB, int& NumC);

/**
 * @brief A delegate that sums two vectors and modifies the result.
 */
DECLARE_DELEGATE_ThreeParams(FActorLocationSum, FVector VectorA, FVector VectorB, FVector& Result);



/**
 * @brief A component that manages delegates for various operations.
 *
 * This class allows for dynamic and multicast delegates, enabling
 * flexible event handling and callback functionality within the game.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, PrioritizeCategories = "DelegateComponent"))
class DELEGATEMODULE_API UDelegateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/**
	 * @brief Constructor for UDelegateComponent.
	 *
	 * Initializes the component and binds various delegates.
	 *
	 * @param ObjectInitializer Initialization parameters for the component.
	 */
	UDelegateComponent(const FObjectInitializer& ObjectInitializer);

	/** A simple delegate for demonstration purposes. */
	FSimpleDelegate SimpleDel;

	/**
	 * @brief A delegate that can be assigned in Blueprints
	 * to respond to changes in an array and string.
	 */
	UPROPERTY(BlueprintAssignable, Category = "DelegateComponent|Delegates")
	FArrayAndStrChange ArrayAndStringChangeDelegate;

	/**
	 * @brief A delegate that broadcasts the current component reference.
	 */
	UPROPERTY(BlueprintAssignable, Category = "DelegateComponent|Delegates")
	FDelegateComponentGet ThisComponentGet;
	
	/**
	 * @brief A delegate that sums two floats and returns the result.
	 */
	UPROPERTY(BlueprintAssignable, Category = "DelegateComponent|Delegates")
	FDFloatSumm DelegateFloatSum;

	/**
	 * @brief Sorts a list of actors using a provided comparator delegate.
	 *
	 * @param ActorList A reference to an array of soft pointers to actors to sort.
	 * @param ActorsComparator A delegate that defines the comparison logic.
	 */
	UFUNCTION(BlueprintCallable, Category = "DelegateComponent|Sort")
	void SortActors(UPARAM(ref) TArray<TSoftObjectPtr<AActor>>& ActorList, FDActorsComparator ActorsComparator);

	/** A multicast delegate for summing integers. */
	DFUsualMulticastSumm UsualMulticastSumm;

	/** A delegate for summing two integers. */
	DFUsualSumm UsualSumm;

	/** A delegate for summing two vectors. */
	FActorLocationSum ActorLocationSum;

	/** A multicast delegate for static summation testing. */
	FSummStaticTest SummStaticTest;

	/**
	 * @brief Sums two integers and stores the result in a reference parameter.
	 *
	 * @param a The first integer.
	 * @param b The second integer.
	 * @param c The result of the sum, passed by reference.
	 */
	UFUNCTION(BlueprintCallable, Category = "SummInt")
	void SummInt(int a, int b, int& c);

	/**
	 * @brief Sums two float values and logs the result.
	 *
	 * @param a The first float value.
	 * @param b The second float value.
	 * @param c The result of the sum, passed by reference.
	 */
	UFUNCTION()
	void Summ(float a, float b, float& c)
	{
		c = a + b;
		UE_LOG(LogTemp, Error, TEXT("Summ is = %f"), c);
	}

protected:
	/**
	 * @brief Called when the game starts or when the component is spawned.
	 *
	 * Initializes any necessary operations at the start of the game.
	 */
	virtual void BeginPlay() override;
private:
	/** Initializes a timer to test delegate functionality. */
	void TestTimerDelegate();

	/**
	 * @brief Executes a threaded delay and binds the result to a delegate upon completion.
	 *
	 * @param DelayDuration The duration to wait before completing the task.
	 * @param ResultDelegate The delegate to execute with the result after the delay.
	 */
	static void MakeThreadedDelay(float DelayDuration, FCallResultDelegate ResultDelegate);
};
