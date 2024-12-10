// This is Sandbox Project.

#include "TagComponent.h"
#include "GameplayTagsManager.h"

#if ! (UE_BUILD_SHIPPING || UE_BUILD_TEST) || USE_LOGGING_IN_SHIPPING
#include "Kismet/KismetSystemLibrary.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(TagComponent)

DEFINE_LOG_CATEGORY_STATIC(TagComponentLog, All, All)

UTagComponent::UTagComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FGameplayTagContainer UTagComponent::GetTagsContainer() const
{
	return TagContainer;
}

bool UTagComponent::AddTagToContainer(const FGameplayTag& Tag)
{
	if (! ContainerHasTag(Tag, true))
	{
		TagContainer.AddTag(Tag);
		UpdateTagMap(Tag, true);

		if (bShouldUpdateParentTagCache)
		{
			UpdateParentTagCache();  // Updating the parent tag cache if required
		}

		OnTagAdded.Broadcast(Tag);

		DEBUG_LOG(TagComponentLog, Log, TEXT("Tag added: %s"), *Tag.ToString());
		return true;
	}

	DEBUG_LOG(TagComponentLog, Warning, TEXT("Tag already exists: %s"), *Tag.ToString());
	return false;
}

TArray<FGameplayTag> UTagComponent::GetArrayTagInContainer() const
{
	return GetTagsContainer().GetGameplayTagArray();
}

void UTagComponent::MergeContainers(const FGameplayTagContainer& TagsToProcess)
{
	if (TagsToProcess.Num() == 0) return;

	BeginBatchOperations();
	
	for (const FGameplayTag& Tag : TagsToProcess)
	{
		AddTagToContainer(Tag);
	}

	EndBatchOperations();
}

bool UTagComponent::RemoveTagFromContainer(const FGameplayTag& Tag)
{
	bool bRemoved = TagContainer.RemoveTag(Tag);

	if (bRemoved)
	{
		UpdateTagMap(Tag, false);

		if (bShouldUpdateParentTagCache)
		{
			UpdateParentTagCache();
		}

		OnTagRemoved.Broadcast(Tag, bRemoved);
		DEBUG_LOG(TagComponentLog, Log, TEXT("Tag removed: %s"), *Tag.ToString());
	}
	else
	{
		DEBUG_LOG(TagComponentLog, Warning, TEXT("Failed to remove tag: %s"), *Tag.ToString());
	}

	return bRemoved;
}

void UTagComponent::RemoveAllTagsFromContainer()
{
	BeginBatchOperations();

	for (const FGameplayTag& Tag : TagContainer)
	{
		RemoveTagFromContainer(Tag);
		DEBUG_LOG(TagComponentLog, Log, TEXT("Remove Tag: %s "), *Tag.ToString());
	}

	EndBatchOperations();
}

bool UTagComponent::ContainerHasAnyTags(const FGameplayTagContainer& TagsToProcess, bool Exact) const
{
	return Exact ? TagContainer.HasAnyExact(TagsToProcess) : TagContainer.HasAny(TagsToProcess);
}

bool UTagComponent::ContainerHasAllTags(const FGameplayTagContainer& TagsToProcess, bool Exact) const
{
	return Exact ? TagContainer.HasAllExact(TagsToProcess) : TagContainer.HasAll(TagsToProcess);
}

bool UTagComponent::ContainerHasTag(const FGameplayTag& Tag, bool Exact) const
{
	return Exact ? TagContainer.HasTagExact(Tag) : TagContainer.HasTag(Tag);
}

FGameplayTag UTagComponent::FindGameplayTagByName(const FName& TagName, bool ErrorIfNotFound) const
{
	// Finding the tag by name in the tag map
	const FGameplayTag* FoundTag = TagMap.Find(TagName);

	if (FoundTag)
	{
		return *FoundTag; // Returning found tag 
	}

	// If the tag was not found and an error is required, log an error
	if (ErrorIfNotFound)
	{
		ensureMsgf(false, TEXT("Tag '%s' not found in the container."), *TagName.ToString());
	}

	return FGameplayTag();
}

bool UTagComponent::DoesContainerHaveExactTag(const FGameplayTag& TagToCheck)
{
	if (! TagToCheck.IsValid())
	{
		DEBUG_LOG(TagComponentLog, Warning, TEXT("Invalid tag provided for exact match check."));
		return false;
	}

	return ContainerHasTag(TagToCheck, true);
}

void UTagComponent::UpdateTagMap(const FGameplayTag& Tag, bool bAdd)
{
	if (bAdd)
	{
		TagMap.Add(Tag.GetTagName(), Tag);
	}
	else
	{
		TagMap.Remove(Tag.GetTagName());
	}
}

void UTagComponent::UpdateParentTagCache() 
{
	if (TagContainer.IsEmpty())
	{
		DEBUG_LOG(TagComponentLog, Warning, TEXT("TagContainer is empty. No parent tags to update."));
		return;
	}

	ParentTagCache.Reset();

	UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();

	for (const FGameplayTag& ParentTag : TagContainer)
	{
		TArray<FGameplayTag> ParentTags;
		TagManager.ExtractParentTags(ParentTag, ParentTags);

		for (const FGameplayTag& Tag : ParentTags)
		{
			ParentTagCache.AddUnique(Tag);
		}
	}
}

FGameplayTagContainer UTagComponent::GetParentTagContainer() const
{
	FGameplayTagContainer ParentTagContainer;

	for (const FGameplayTag& ParentTag : ParentTagCache)
	{
		ParentTagContainer.AddTag(ParentTag);
	}

	return ParentTagContainer;
}

FGameplayTagContainer UTagComponent::GetParentTagArrayByName(const FName& TagName) const
{
	FGameplayTag ChildTag = FindGameplayTagByName(TagName, false);

	if (! ChildTag.IsValid())
	{
		DEBUG_LOG(TagComponentLog, Log, TEXT("Tag not found: %s"), *TagName.ToString());
		return FGameplayTagContainer();
	}

	return ChildTag.GetGameplayTagParents();
}

void UTagComponent::BeginBatchOperations()
{
	bShouldUpdateParentTagCache = false;
}

void UTagComponent::EndBatchOperations()
{
	bShouldUpdateParentTagCache = true;
	UpdateParentTagCache();
}

#if WITH_EDITORONLY_DATA

void UTagComponent::DebugTagContainer()
{
	// Debugging: printing each tag in the container to the log and the screen
	for (FGameplayTag Tag : GetArrayTagInContainer())
	{
		DEBUG_LOG(TagComponentLog, Log, TEXT("Tag Name : %s"), *Tag.ToString());
		UKismetSystemLibrary::PrintString(GetWorld(), *Tag.ToString());
	}
}

#endif