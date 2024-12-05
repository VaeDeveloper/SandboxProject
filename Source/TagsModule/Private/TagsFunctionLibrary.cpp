// This is Sandbox Project. 


#include "TagsFunctionLibrary.h"
#include "TagComponent.h"

UTagComponent* UTagsFunctionLibrary::GetTagComponent(UObject* WorldContextObject) 
{
	AActor* Owner = GetOwner(WorldContextObject);
	if (! Owner) return nullptr;

	return Owner->FindComponentByClass<UTagComponent>();
}

FGameplayTagContainer UTagsFunctionLibrary::GetTagContainer(UObject* WorldContextObject)
{
	UTagComponent* TagComponent = GetTagComponent(WorldContextObject);
	if (! TagComponent) return FGameplayTagContainer{};

	return TagComponent->GetTagsContainer();
}

TArray<FGameplayTag> UTagsFunctionLibrary::GetBreakTags(UObject* WorldContextObject)
{
	UTagComponent* TagComponent = GetTagComponent(WorldContextObject);
	if (! TagComponent) return TArray<FGameplayTag>();

	TArray<FGameplayTag> GameplayTags = TagComponent->GetTagsContainer().GetGameplayTagArray();
	if (GameplayTags.IsEmpty()) return TArray<FGameplayTag>();

	return GameplayTags;
}

bool UTagsFunctionLibrary::AddTagToComponentTagContainer(UObject* WorldContextObject, const FGameplayTag& Tag)
{
	UTagComponent* TagComponent = GetTagComponent(WorldContextObject);
	if (! TagComponent || !Tag.IsValid()) return false;
	
	TagComponent->AddTagToContainer(Tag);
	return true;
}




