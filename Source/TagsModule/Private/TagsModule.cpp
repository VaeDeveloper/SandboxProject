#include "TagsModule.h"

DEFINE_LOG_CATEGORY(TagsModule);

#define LOCTEXT_NAMESPACE "FTagsModule"

void FTagsModule::StartupModule()
{
	UE_LOG(TagsModule, Warning, TEXT("TagsModule module has been loaded"));
}

void FTagsModule::ShutdownModule()
{
	UE_LOG(TagsModule, Warning, TEXT("TagsModule module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTagsModule, TagsModule)