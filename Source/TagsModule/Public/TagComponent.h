// This is Sandbox Project.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "TagComponent.generated.h"

/**
 * This macro defines logging behavior for debugging purposes.
 *
 * Depending on the build configuration (e.g., Shipping or Test build),
 * it either enables or disables logging. It allows conditional logging
 * in non-shipping builds (e.g., Development, Debug) while ensuring that
 * log statements are omitted in Shipping builds to improve performance and
 * avoid leaking debug information.
 *
 * In shipping or test builds, the logging is disabled by default, unless
 * the `USE_LOGGING_IN_SHIPPING` preprocessor directive is defined.
 *
 * Usage:
 *  - In non-shipping builds, logs are printed using `UE_LOG` with the provided category, verbosity, and format.
 *  - In shipping builds, this macro is no-op and will not generate any log output.
 *
 * @param CategoryName The log category (e.g., TagComponentLog) to use for the log message.
 * @param Verbosity The verbosity level (e.g., Log, Warning, Error) of the log message.
 * @param Format The format string, similar to `printf` formatting, that specifies how the log message should be formatted.
 * @param ... Additional arguments to pass into the format string, as in `printf`.
 */
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
	void RemoveAllTagsFromContainer();

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
	FGameplayTag FindGameplayTagByName(const FName& TagName, bool ErrorIfNotFound = false) const;

	/**
	 * Checks if a tag that exactly matches the provided tag exists in the container.
	 *
	 * @param TagContainer The container to search for the tag.
	 * @param TagToCheck The tag to look for.
	 * @return True if a tag that exactly matches TagToCheck is found in the container, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "TagsComponent", meta = (AutoCreateRefTerm = "TagToCheck"))
	bool DoesContainerHaveExactTag(const FGameplayTag& TagToCheck);

	/**
	 * Retrieves the container holding all parent tags for the current tag container.
	 *
	 * This function returns the full set of parent tags for the tags managed by this component.
	 *
	 * @return A FGameplayTagContainer containing all the parent tags.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTagContainer GetParentTagContainer() const;

	/**
	 * Retrieves the parent tags associated with a specific tag identified by its name.
	 *
	 * This function looks up the tag by its name and returns all the parent tags for the found tag.
	 * If the tag is not found, it returns an empty container.
	 *
	 * @param TagName The name of the tag whose parent tags are to be retrieved.
	 * @return A FGameplayTagContainer containing the parent tags of the specified tag.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TagsComponent", meta = (AutoCreateRefTerm = "TagName"))
	FGameplayTagContainer GetParentTagArrayByName(const FName& TagName) const;


#if WITH_EDITORONLY_DATA

	/**
	 * Debug function to print and inspect the tag container.
	 *
	 * This function will output all tags currently in the tag container for debugging purposes.
	 * It is available only in the editor.
	 */
	UFUNCTION(BlueprintCallable, Category = "TagsComponent|Debug")
	void DebugTagContainer();

#endif

	/**
	 * The container that holds all gameplay tags managed by this component.
	 *
	 * This container stores the tags added to the component. It is used to keep track of all active gameplay tags
	 * and is used in various functions to add, remove, or query tags.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TagsComponent|GameplayTags", meta = (DisplayName = "TagsContainer"))
	FGameplayTagContainer TagContainer;

	/**
	 * Delegate triggered when a tag is added to the container.
	 *
	 * This delegate is broadcasted whenever a tag is successfully added to the TagContainer.
	 */
	UPROPERTY(BlueprintAssignable, Category = "TagsComponent|Delegated")
	FOnTagAddedDelegate OnTagAdded;

	/**
	 * Delegate triggered when a tag is removed from the container.
	 *
	 * This delegate is broadcasted whenever a tag is successfully removed from the TagContainer.
	 */
	UPROPERTY(BlueprintAssignable, Category = "TagsComponent|Delegated")
	FOnTagRemovedDelegate OnTagRemoved;

private:
	/**
	 * Updates the internal map with the tag, either adding or removing it based on the flag.
	 *
	 * This method ensures that the internal map, TagMap, is updated whenever a tag is added or removed. The update
	 * operation will add or remove the tag based on the provided flag (bAdd).
	 *
	 * @param Tag The tag to be added or removed from the map.
	 * @param bAdd Whether to add (true) or remove (false) the tag from the map.
	 */
	void UpdateTagMap(const FGameplayTag& Tag, bool bAdd);

	/**
	 * Updates the cache of parent tags for the current tag container.
	 *
	 * This method updates the ParentTagCache to reflect the latest set of parent tags associated with the tags
	 * in the TagContainer. It is typically called when tags are added or removed.
	 */
	void UpdateParentTagCache();

	/**
	 * Begins a batch operation for tag management.
	 *
	 * This method is used to prepare for a batch of operations on the tag container, temporarily disabling automatic
	 * updates to the parent tag cache during the batch.
	 */
	void BeginBatchOperations();

	/**
	 * Ends the batch operation and restores normal behavior.
	 *
	 * This method finalizes the batch operation, re-enables the automatic updating of the parent tag cache, and
	 * ensures the cache is updated accordingly.
	 */
	void EndBatchOperations();

	/**
	 * A flag indicating whether the parent tag cache should be updated after batch operations.
	 *
	 * This flag is used to control whether the parent tag cache is updated during or after batch operations.
	 */
	bool bShouldUpdateParentTagCache = true;

	/**
	 * A map that holds all the tags by their name for fast look-up.
	 *
	 * The TagMap is used for efficient retrieval of gameplay tags based on their name. It helps in checking if a
	 * specific tag exists or needs to be added to the container.
	 */
	TMap<FName, FGameplayTag> TagMap;

	/**
	 * A cache of parent tags for efficient access and management.
	 *
	 * The ParentTagCache holds the parent tags for all tags in the container. This cache is updated whenever
	 * the tag container is modified and is used to quickly retrieve parent tag data.
	 */
	TArray<FGameplayTag> ParentTagCache;
};
