// This is Sandbox Project.

#include "TagComponent.h"
#include "GameplayTagsManager.h"

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
		OnTagAdded.Broadcast(Tag);

		UE_LOG(TagComponentLog, Log, TEXT("Tag added: %s"), *Tag.ToString());
		return true;
	}

	UE_LOG(TagComponentLog, Warning, TEXT("Tag already exists: %s"), *Tag.ToString());
	return false;
}

void UTagComponent::MergeContainers(const FGameplayTagContainer& TagsToProcess)
{
	if (TagsToProcess.Num() == 0) return;

	for (const FGameplayTag& Tag : TagsToProcess)
	{
		AddTagToContainer(Tag);
	}
}

bool UTagComponent::RemoveTagFromContainer(const FGameplayTag& Tag)
{
	bool bRemoved = TagContainer.RemoveTag(Tag);

	if (bRemoved)
	{
		UpdateTagMap(Tag, false);
		OnTagRemoved.Broadcast(Tag, bRemoved);
		UE_LOG(TagComponentLog, Log, TEXT("Tag removed: %s"), *Tag.ToString());
	}
	else
	{
		UE_LOG(TagComponentLog, Warning, TEXT("Failed to remove tag: %s"), *Tag.ToString());
	}

	return bRemoved;
}

void UTagComponent::RemoveAllTagsFromContainer(const FGameplayTagContainer& TagsToProcess)
{
	for (const FGameplayTag& Tag : TagsToProcess)
	{
		RemoveTagFromContainer(Tag);
		UE_LOG(TagComponentLog, Log, TEXT("Remove Tag: %s "), *Tag.ToString());
	}
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

FGameplayTag UTagComponent::FindGameplayTagByName(const FName& TagName, bool ErrorIfNotFound)
{
	const FGameplayTag* FoundTag = TagMap.Find(TagName);

	if (FoundTag)
	{
		return *FoundTag;
	}

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
		UE_LOG(LogTemp, Warning, TEXT("Invalid tag provided for exact match check."));
		return false;
	}

	for (const FGameplayTag& Tag : TagContainer)
	{
		if (Tag.MatchesTagExact(TagToCheck))
		{
			return true;
		}
	}

	return false;
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
		UE_LOG(LogTemp, Warning, TEXT("TagContainer is empty. No parent tags to update."));
		return;
	}

	ParentTagCache.Empty();
	FGameplayTagContainer ParentTags = TagContainer.GetGameplayTagParents();

	for (const FGameplayTag& ParentTag : ParentTags)
	{
		if (ParentTag.IsValid())
		{
			ParentTagCache.Add(ParentTag);
		}
	}
}


