// This is Sandbox Project.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "TagsFunctionLibrary.generated.h"

class UTagComponent;


/**
 *
 */
UCLASS()
class TAGSMODULE_API UTagsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * 
	 */
	static inline AActor* GetOwner(UObject* Context)
	{
		AActor* Owner = Cast<AActor>(Context);
		if (! Owner)
		{
			UActorComponent* OwningComponent = Cast<UActorComponent>(Owner);
			if (! OwningComponent) return nullptr;

			Owner = OwningComponent->GetOwner();
		}

		return Owner;
	}

public:
	/**
	 *
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tags Library", meta = (WorldContext = "WorldContextObject"))
	static UTagComponent* GetTagComponent(UObject* WorldContextObject);

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tags Library", meta = (WorldContext = "WorldContextObject"))
	static FGameplayTagContainer GetTagContainer(UObject* WorldContextObject);
	
	/**
	 *
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tags Library", meta = (WorldContext = "WorldContextObject"))
	static TArray<FGameplayTag> GetBreakTags(UObject* WorldContextObject);

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable, Category = "Tags Library", meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "Tag"))
	bool AddTagToComponentTagContainer(UObject* WorldContextObject, const FGameplayTag& Tag);
};
