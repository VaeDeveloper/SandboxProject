// This is Sandbox Project.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "TagComponent.generated.h"



#if ! (UE_BUILD_SHIPPING || UE_BUILD_TEST) || USE_LOGGING_IN_SHIPPING
#define DEBUG_LOG(CategoryName, Verbosity, Format, ...) UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__)
#else
#define DEBUG_LOG(CategoryName, Verbosity, Format, ...)
#endif

/**
 * Dynamic multicast delegate triggered when a tag is added.
 *
 * @param Tag The gameplay tag that was added.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTagAddedDelegate, FGameplayTag, Tag);

/**
 * Dynamic multicast delegate triggered when a tag is removed.
 *
 * @param Tag The gameplay tag that was removed.
 * @param bIsRemoved Indicates whether the removal was successful.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTagRemovedDelegate, FGameplayTag, Tag, bool, bIsRemoved);

/**
 * A component that manages a container of gameplay tags.
 * Provides functionality to add, remove, and query tags,
 * with support for Blueprint integration and event broadcasting.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, PrioritizeCategories = "TagsComponent", DisplayName = "Tags Component"))
class TAGSMODULE_API UTagComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Default constructor. */
	UTagComponent();

	/**
	 * Retrieves the current container of gameplay tags.
	 *
	 * @return A copy of the current FGameplayTagContainer.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TagsComponent")
	FORCEINLINE FGameplayTagContainer GetTagsContainer() const;

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TagsComponent")
	TArray<FGameplayTag> GetArrayTagInContainer() const;

	/**
	 * Adds a tag to the container if it doesn't already exist.
	 *
	 * @param Tag The gameplay tag to add.
	 * @return True if the tag was successfully added; false if it already exists.
	 */
	UFUNCTION(BlueprintCallable, Category = "Tags Component", meta = (AutoCreateRefTerm = "Tag"))
	bool AddTagToContainer(const FGameplayTag& Tag);

	/**
	 * Merges another container's tags into this container.
	 * Only unique tags will be added.
	 *
	 * @param inTagsContainer The container whose tags will be merged.
	 */
	UFUNCTION(BlueprintCallable, Category = "TagsComponent")
	void MergeContainers(const FGameplayTagContainer& TagsToProcess);

	/**
	 * Removes a specific tag from the container.
	 *
	 * @param Tag The gameplay tag to remove.
	 * @return True if the tag was successfully removed; false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "TagsComponent", meta = (AutoCreateRefTerm = "Tag"))
	bool RemoveTagFromContainer(const FGameplayTag& Tag);

	/**
	 * Removes all specified tags from the container.
	 *
	 * @param inTagsContainer A container of tags to remove.
	 */
	UFUNCTION(BlueprintCallable, Category = "TagsComponent")
	void RemoveAllTagsFromContainer(const FGameplayTagContainer& TagsToProcess);

	/**
	 * Checks if the container has any tags from the specified container.
	 *
	 * @param inTagsContainer The container of tags to check.
	 * @param Exact If true, requires an exact match; otherwise, allows partial matches.
	 * @return True if any tags match; false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TagsComponent")
	FORCEINLINE bool ContainerHasAnyTags(const FGameplayTagContainer& TagsToProcess, bool Exact = false) const;

	/**
	 * Checks if the container contains all tags from the specified container.
	 *
	 * @param inTagsContainer The container of tags to check.
	 * @param Exact If true, requires an exact match; otherwise, allows partial matches.
	 * @return True if all tags match; false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TagsComponent", meta = (AutoCreateRefTerm = "TagsToProcess"))
	FORCEINLINE bool ContainerHasAllTags(const FGameplayTagContainer& TagsToProcess, bool Exact = false) const;

	/**
	 * Checks if the container contains a specific tag.
	 *
	 * @param Tag The gameplay tag to check.
	 * @param Exact If true, requires an exact match; otherwise, allows partial matches.
	 * @return True if the tag is present; false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TagsComponent", meta = (AutoCreateRefTerm = "Tag"))
	FORCEINLINE bool ContainerHasTag(const FGameplayTag& Tag, bool Exact = false) const;

	/**
	 * Finds an FGameplayTag in the provided FGameplayTagContainer by its name.
	 *
	 * @param TagContainer The container to search in.
	 * @param TagName The name of the tag to find.
	 * @param ErrorIfNotFound If true, ensures the tag exists in the container.
	 * @return The found FGameplayTag, or an empty FGameplayTag if not found.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TagsComponent", meta = (AutoCreateRefTerm = "TagName"))
	FGameplayTag FindGameplayTagByName(const FName& TagName, bool ErrorIfNotFound = false);

	/**
	 * Checks if a tag that exactly matches the provided tag exists in the container.
	 *
	 * @param TagContainer The container to search for the tag.
	 * @param TagToCheck The tag to look for.
	 * @return True if a tag that exactly matches TagToCheck is found in the container, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "TagsComponent", meta = (AutoCreateRefTerm = "TagToCheck"))
	bool DoesContainerHaveExactTag(const FGameplayTag& TagToCheck);

#if WITH_EDITORONLY_DATA

	UFUNCTION(BlueprintCallable, Category = "TagsComponent|Debug")
	void DebugTagContainer();

#endif

public:
	/** The container that holds all gameplay tags managed by this component. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TagsComponent|GameplayTags", meta = (DisplayName = "TagsContainer"))
	FGameplayTagContainer TagContainer;

	/** Delegate triggered when a tag is added to the container. */
	UPROPERTY(BlueprintAssignable, Category = "TagsComponent|Delegated")
	FOnTagAddedDelegate OnTagAdded;

	/** Delegate triggered when a tag is removed from the container. */
	UPROPERTY(BlueprintAssignable, Category = "TagsComponent|Delegated")
	FOnTagRemovedDelegate OnTagRemoved;

private:
	void UpdateTagMap(const FGameplayTag& Tag, bool bAdd);
	void UpdateParentTagCache();

	TMap<FName, FGameplayTag> TagMap;
	TSet<FGameplayTag> ParentTagCache;
};
