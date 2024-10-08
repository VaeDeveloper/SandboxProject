#include "ThreadsModule.h"

DEFINE_LOG_CATEGORY(ThreadsModule);

#define LOCTEXT_NAMESPACE "FThreadsModule"

void FThreadsModule::StartupModule()
{
	UE_LOG(ThreadsModule, Warning, TEXT("ThreadsModule module has been loaded"));
}

void FThreadsModule::ShutdownModule()
{
	UE_LOG(ThreadsModule, Warning, TEXT("ThreadsModule module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FThreadsModule, ThreadsModule)