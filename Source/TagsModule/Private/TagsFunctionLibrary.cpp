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

	return GameplayTags;
}




